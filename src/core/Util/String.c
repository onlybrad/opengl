#include <string.h>
#include "String.h"
#include "util.h"

String Str_init(const char *c_str) {
    return (String){
        .buffer = unconst(c_str),
        .length = strlen(c_str)
    };
}

String Str_init_move(const char *c_str, const size_t length) {
    return (String){
        .buffer = unconst(c_str),
        .length = length
    };
}

String Str_init_copy(const char *c_str, const size_t length) {
    return (String) {
        .buffer = strdup(c_str),
        .length = length
    };
}

void String_free(String str) {
    free(unconst(str.buffer));
}

size_t string_hash_function(const char *key) {
    size_t hash = 0xcbf29ce484222325ULL;
    size_t str_len = strlen(key);
    for(size_t i=0; i < str_len; i++) {
        hash = (hash ^ (size_t)key[i]) * 0x100000001b3ULL;
    }

    return hash;
}

bool string_compare(const char *str1, const char *str2) {
    if(str1 == str2) {
        return true;
    }

    if(str1 == NULL || str2 == NULL) {
        return false;
    }

    return strcmp(str1, str2) == 0;
}
