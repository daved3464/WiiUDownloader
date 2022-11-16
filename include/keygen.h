#pragma once

#include <gtitles.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool generate_key(const char *title_id, char *output);
int char2int(char input);
void hex2bytes(const char *input, uint8_t *output);
void hex(uint64_t i, int digits, char *out);

#ifdef __cplusplus
}
#endif
