#pragma once

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

void setup_progress_dialog(GtkWindow *window, GtkProgressBar *progress_bar);

int cdecrypt(int argc, char **argv, GtkWindow *window, GtkProgressBar *progress_bar);

#ifdef __cplusplus
}
#endif