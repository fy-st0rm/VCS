#ifndef _UTIL_H_
#define _UTIL_H_

#include "const.h"

bool file_exists(char* path, char* file);
void recur_list_dir(char* path, char** in, char*** out, int* out_idx);

char* load_file(char* file);

#endif
