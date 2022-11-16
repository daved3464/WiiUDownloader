#pragma once

#include <stdbool.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

int download_title(GtkWindow *window, const char *title_id, bool decrypt);

#ifdef __cplusplus
}
#endif