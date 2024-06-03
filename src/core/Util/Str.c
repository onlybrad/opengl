#include <string.h>
#include "Str.h"

size_t string_hash_function(str key) {
    size_t hash = 0xcbf29ce484222325ULL;
    size_t str_len = strlen(key);
    for(size_t i=0; i < str_len; i++) {
        hash = (hash ^ (size_t)key[i]) * 0x100000001b3ULL;
    }

    return hash;
}

bool string_compare(str str1, str str2) {
    if(str1 == str2) {
        return true;
    }

    if(str1 == NULL || str2 == NULL) {
        return false;
    }

    return strcmp(str1, str2) == 0;
}
