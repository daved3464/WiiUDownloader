#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef _WIN32

#include <sys/stat.h>

#endif

#include <cdecrypt/cdecrypt.h>
#include <downloader.h>
#include <keygen.h>

#include <curl/curl.h>
#include <gtk/gtk.h>

#include <pthread.h>

struct MemoryStruct {
    uint8_t *memory;
    size_t size;
};

struct PathFileStruct {
    char *file_path;
    FILE *file_pointer;
};

static char currentFile[255] = "None";
static char *selected_dir = NULL;

static inline uint16_t bswap_16(uint16_t value) {
    return (uint16_t) ((0x00FF & (value >> 8)) | (0xFF00 & (value << 8)));
}

static inline uint32_t bswap_32(uint32_t value) {
    return value >> 24 | (value >> 8 & 0xff00) | (value << 8 & 0xff0000) | value << 24;
}

static char *readable_fs(double size, char *buf) {
    int i = 0;
    const char *units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    sprintf(buf, "%.*f %s", i, size, units[i]);
    return buf;
}

//LibCurl progress function
int curl_progress_handler(void *p,
                          curl_off_t dltotal, curl_off_t dlnow,
                          curl_off_t ultotal, curl_off_t ulnow) {

    if (dltotal == 0)
        dltotal = 1;
    if (dlnow == 0)
        dlnow = 1;

    GtkProgressBar *progress_bar = (GtkProgressBar *) p;

    char downloadString[255];
    char downNow[255];
    char downTotal[255];
    readable_fs(dlnow, downNow);
    readable_fs(dltotal, downTotal);
    sprintf(downloadString, "Downloading %s (%s/%s)", currentFile, downNow, downTotal);

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), (double) dlnow / (double) dltotal);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), downloadString);
    // force redraw
    while (g_main_context_pending(g_main_context_default()))
        g_main_context_iteration(g_main_context_default(), false);
    return 0;
}

static size_t write_data_to_memory(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    mem->memory = realloc(mem->memory, mem->size + realsize);
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;

    return realsize;
}

static void create_ticket(
        const char *title_id, const char *title_key, uint16_t title_version, const char *output_path) {
    FILE *ticket_file = fopen(output_path, "wb");
    if (!ticket_file) {
        fprintf(stderr, "Error: The file \"%s\" couldn't be opened. Will exit now.\n", output_path);
        exit(EXIT_FAILURE);
    }

    uint8_t ticket_data[848] = "\x00\x01\x00\x04\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\xd1\x5e\xa5\xed\x15\xab\xe1\x1a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x52\x6f\x6f\x74\x2d\x43\x41\x30\x30\x30\x30\x30\x30\x30\x33\x2d\x58\x53\x30\x30\x30\x30\x30\x30\x30\x63\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\xfe\xed\xfa\xce\x01\x00\x00\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x14\x00\x00\x00\xac\x00\x00\x00\x14\x00\x01\x00\x14\x00\x00\x00\x00\x00\x00\x00\x28\x00\x00\x00\x01\x00\x00\x00\x84\x00\x00\x00\x84\x00\x03\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    // fill in the values from the title_id and title_key
    hex2bytes(title_id, &ticket_data[476]);
    hex2bytes(title_key, &ticket_data[447]);
    memcpy(&ticket_data[486], &title_version, 2);
    fwrite(ticket_data, 1, 848, ticket_file);
    fclose(ticket_file);
    printf("Finished creating \"%s\".\n", output_path);
}

static void prepend(char *s, const char *t) {
    size_t len = strlen(t);
    memmove(s + len, s, strlen(s) + 1);
    memcpy(s, t, len);
}

static char *dirname(char *path) {
    int len = strlen(path);
    int last = len - 1;
    char *parent = malloc(sizeof(char) * (len + 1));
    strcpy(parent, path);
    parent[len] = '\0';

    while (last >= 0) {
        if (parent[last] == '/') {
            parent[last] = '\0';
            break;
        }
        last--;
    }
    return parent;
}


static void on_dialog_response(GtkDialog *dialog, gint response_id, char *folder_path) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        GFile *file = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(dialog));
        sprintf(folder_path, "%s", g_file_get_path(file));
    }
}

// function to return the path of the selected folder
static char *show_folder_select_dialog() {
    GtkFileChooserNative *dialog;
    char *folder_path = NULL;

    dialog = gtk_file_chooser_native_new(
            "Select a folder",
            NULL,
            GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
            "_Select",
            "_Cancel"
    );

    g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(on_dialog_response), folder_path);

    return folder_path;
}

static void download_certificate(const char *outputPath) {
    FILE *cetk = fopen(outputPath, "wb");
    if (cetk == NULL)
        return;
    CURL *certHandle = curl_easy_init();
    curl_easy_setopt(certHandle, CURLOPT_FAILONERROR, 1L);

    // Download the tmd and save it in memory, as we need some data from it
    curl_easy_setopt(certHandle, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(certHandle, CURLOPT_URL,
                     "http://ccs.cdn.c.shop.nintendowifi.net/ccs/download/000500101000400A/cetk");

    curl_easy_setopt(certHandle, CURLOPT_WRITEDATA, cetk);
    curl_easy_perform(certHandle);
    curl_easy_cleanup(certHandle);
    fclose(cetk);
}

static int download_file(GtkProgressBar *progress_bar, const char *download_url, const char *output_path) {
    // Try to open file
    FILE *file = fopen(output_path, "wb");
    if (file == NULL)
        return 1;

    CURL *handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_FAILONERROR, 1L);

    // Download the tmd and save it in memory, as we need some data from it
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(handle, CURLOPT_URL, download_url);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, curl_progress_handler);
    curl_easy_setopt(handle, CURLOPT_PROGRESSDATA, progress_bar);

    curl_easy_setopt(handle, CURLOPT_WRITEDATA, file);
    curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    fclose(file);
    return 0;
}

int download_title(GtkWindow *window, const char *title_id, bool decrypt) {
    // initialize some useful variables
    char *output_dir = malloc(1024);

    strcpy(output_dir, title_id);
    prepend(output_dir, "/");

    if (selected_dir == NULL)
        selected_dir = show_folder_select_dialog();

    if (selected_dir == NULL) {
        free(output_dir);
        return 0;
    }

    prepend(output_dir, selected_dir);
    if (output_dir[strlen(output_dir) - 1] == '/' || output_dir[strlen(output_dir) - 1] == '\\') {
        output_dir[strlen(output_dir) - 1] = '\0';
    }

    char output_path[strlen(output_dir) + 14];
    char download_url[81];
    char base_url[69];

    snprintf(base_url, 69, "http://ccs.cdn.c.shop.nintendowifi.net/ccs/download/%s", title_id);

    // create the output directory if it doesn't exist
#ifdef _WIN32
    mkdir(output_dir);
#else
    mkdir(output_dir, 0700);
#endif

    // initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    // -- Title Metadata (TMD) fetching --
    struct MemoryStruct title_metadata;
    title_metadata.memory = malloc(0);
    title_metadata.size = 0;

    // set download url
    snprintf(download_url, 73, "%s/%s", base_url, "tmd");

    // make an own handle for the tmd file, as we want to download it to memory first
    CURL *curl_tmd_handler = curl_easy_init();

    curl_easy_setopt(curl_tmd_handler, CURLOPT_FAILONERROR, 1L);

    // Download the tmd and save it in memory, as we need some data from it
    curl_easy_setopt(curl_tmd_handler, CURLOPT_URL, download_url);
    curl_easy_setopt(curl_tmd_handler, CURLOPT_WRITEDATA, (void *) &title_metadata);
    curl_easy_setopt(curl_tmd_handler, CURLOPT_WRITEFUNCTION, write_data_to_memory);
    curl_easy_perform(curl_tmd_handler);
    curl_easy_cleanup(curl_tmd_handler);

    // -- Handle downloaded TMD file --
    // Download title certificate
    snprintf(output_path, sizeof(output_path), "%s/%s", output_dir, "title.cert");
    download_certificate(output_path);

    // write out the tmd file
    snprintf(output_path, sizeof(output_path), "%s/%s", output_dir, "title.tmd");
    FILE *tmd_file = fopen(output_path, "wb");

    if (!tmd_file) {
        free(output_dir);
        fprintf(stderr, "Error: The file \"%s\" couldn't be opened. Will exit now.\n", output_path);
        exit(EXIT_FAILURE);
    }

    fwrite(title_metadata.memory, 1, title_metadata.size, tmd_file);
    fclose(tmd_file);
    printf("Finished downloading \"%s\".\n", output_path);

    // Setup Title Version
    uint16_t title_version;
    memcpy(&title_version, &title_metadata.memory[476], 2);
    snprintf(output_path, sizeof(output_path), "%s/%s", output_dir, "title.tik");

    // Setup Title Key
    char title_key[128];
    generate_key(title_id, title_key);

    // Create Ticket
    create_ticket(title_id, title_key, title_version, output_path);

    uint16_t content_count;
    memcpy(&content_count, &title_metadata.memory[478], 2);
    content_count = bswap_16(content_count);

    // Show a progress dialog
    GtkWidget *progress_bar = gtk_progress_bar_new();
    setup_progress_dialog(window, GTK_PROGRESS_BAR(progress_bar));

    // Add all needed curl handles to the multi handle
    for (int i = 0; i < content_count; i++) {
        int offset = 2820 + (48 * i);
        uint32_t id; // the id should usually be chronological, but we wanna be sure
        memcpy(&id, &title_metadata.memory[offset], 4);
        id = bswap_32(id);

        // add a curl handle for the content file (.app file)
        snprintf(output_path, sizeof(output_path), "%s/%08X.app", output_dir, id);
        snprintf(download_url, 78, "%s/%08X", base_url, id);
        sprintf(currentFile, "%08X.app", id);
        download_file(GTK_PROGRESS_BAR(progress_bar), download_url, output_path);

        if ((title_metadata.memory[offset + 7] & 0x2) == 2) {
            // add a curl handle for the hash file (.h3 file)
            snprintf(output_path, sizeof(output_path), "%s/%08X.h3", output_dir, id);
            snprintf(download_url, 81, "%s/%08X.h3", base_url, id);
            sprintf(currentFile, "%08X.h3", id);
            download_file(GTK_PROGRESS_BAR(progress_bar), download_url, output_path);
        }
    }

    free(title_metadata.memory);

    printf("Downloading all files for TitleID %s done...\n", title_id);

    // Post operation cleanup
    curl_global_cleanup();
    gtk_window_destroy(window);
    if (decrypt) {
        char *argv[2] = {"WiiUDownloader", dirname(output_path)};
        GtkWidget *w = gtk_window_new();
        cdecrypt(2, argv,
                 GTK_WINDOW(w),
                 GTK_PROGRESS_BAR(gtk_progress_bar_new()));

        gtk_window_destroy(w);
    }

    free(output_dir);
}