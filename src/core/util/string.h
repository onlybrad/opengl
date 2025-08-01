#ifndef OB_STRING_H
#define OB_STRING_H

#include <stdlib.h>
#include <stdbool.h>

struct OB_String {
    const char *buffer;
    size_t length;
};

struct OB_String String_init(const char *c_str);
struct OB_String String_init_copy(const char *c_str);
void OB_String_free(struct OB_String str);

size_t OB_String_hash(const char *key);
bool OB_String_compare(const char *str1, const char *str2);

#endif