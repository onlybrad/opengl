#ifndef _STR_H
#define _STR_H

#include <stdlib.h>
#include <stdbool.h>

typedef const char *str;

size_t string_hash_function(str key);
bool string_compare(str str1, str str2);

#endif