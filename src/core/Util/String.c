#include <string.h>
#include "string.h"
#include "util.h"

struct OB_String String_init(const char *c_str) {
    struct OB_String string = {c_str, strlen(c_str)};
    return string;
}

struct OB_String String_init_copy(const char *c_str) {
    struct OB_String string = {strdup(c_str), strlen(c_str)};
    return string;
}

void OB_String_free(struct OB_String str) {
    free(OB_unconst(str.buffer));
    str.buffer = NULL;
    str.length = 0;
}

size_t OB_String_hash(const char *key) {
    size_t hash = 0xcbf29ce484222325;
    const size_t str_len = strlen(key);
    for(size_t i = 0; i < str_len; i++) {
        hash = (hash ^ (size_t)key[i]) * 0x100000001b3;
    }

    return hash;
}

bool OB_String_compare(const char *str1, const char *str2) {
    if(str1 == str2) {
        return true;
    }

    if(str1 == NULL || str2 == NULL) {
        return false;
    }

    return strcmp(str1, str2) == 0;
}
