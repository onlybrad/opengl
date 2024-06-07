#ifndef _STRING_H
#define _STRING_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct String {
    const char *buffer;
    size_t length;
} String;

String Str_init(const char *c_str);
String Str_init_move(const char *c_str, const size_t length);
String Str_init_copy(const char *c_str, const size_t length);
void String_free(String str);

size_t string_hash_function(const char * key);
bool string_compare(const char * str1, const char * str2);

#endif