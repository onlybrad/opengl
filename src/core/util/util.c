#include <string.h>
#include <assert.h>
#include "util.h"

FILE *OB_file_open(const char *filename, const char *mode) {
    #if _WIN32
    wchar_t modew[64];
    wchar_t filenamew[1024];

    if(MultiByteToWideChar(CP_UTF8, 0ul, filename, -1, filenamew, OB_ARRAY_LEN(filenamew)) == 0) {
        return NULL;
    }

    if(MultiByteToWideChar(CP_UTF8, 0ul, mode, -1, modew, OB_ARRAY_LEN(modew)) == 0) {
        return NULL;
    }

    return _wfopen(filenamew, modew);
    #else   
    return fopen(filename, mode);
    #endif
}

struct OB_String OB_file_get_contents(const char *path) {
    FILE *const file = OB_file_open(path, "rb");
    struct OB_String str = {NULL, 0};

    if (!file) {
        return str;
    }

    fseeko(file, 0, SEEK_END);
    str.length = (size_t)ftello(file);
    fseeko(file, 0, SEEK_SET);
    str.buffer = (const char*)malloc(str.length * sizeof(char));

    if(str.buffer == NULL) {
        str.buffer = NULL;
        str.length = 0;
        return str;
    }

    char *buffer = (char*)OB_unconst(str.buffer);
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, str.length, file)) > 0);

    if(ferror(file)) {
        free(buffer);
        str.buffer = NULL;
        str.length = 0;
    }

    fclose(file);
    
    return str;
}

/**
 * @brief this function will convert const to non-const pointer without triggering the -Wcast-qual warning in GCC. It is useful for using with free() which doesn't accept a const pointer without GCC issuing a warning.
 */
void *OB_unconst(const void *const_var) {
    union {
        const void *const_var;
        void *unconst_var;
    } var;

    var.const_var = const_var;
    return var.unconst_var;
}