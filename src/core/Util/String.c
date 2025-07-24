#include <string.h>
#include "String.h"
#include "util.h"

String String_init(const char *c_str) {
    String string = {c_str, strlen(c_str)};
    return string;
}

String String_init_copy(const char *c_str) {
    String string = {strdup(c_str), strlen(c_str)};
    return string;
}

void String_free(String str) {
    free(unconst(str.buffer));
}

size_t string_hash_function(const char *key) {
    size_t hash = 0xcbf29ce484222325;
    const size_t str_len = strlen(key);
    for(size_t i = 0; i < str_len; i++) {
        hash = (hash ^ (size_t)key[i]) * 0x100000001b3;
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
