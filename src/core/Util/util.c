#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

String file_get_contents(const char *const path) {
    FILE *const file = fopen(path, "rb");

    if (!file) {
        perror(NULL);
        return (String){
            .buffer = NULL,
            .length = 0
        };
    }

    String str;

    fseek(file, 0, SEEK_END);
    str.length = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);
    str.buffer = malloc(str.length * sizeof(char));
    fread(unconst(str.buffer), 1, str.length, file);
    fclose(file);
    
    return str;
}

void *unconst(const void *var) {
    void *unconst_var;
    memcpy(&unconst_var, &var, sizeof(void*));
    
    return unconst_var;
}

// GLsizei get_size_type(const GLenum type) {
//     switch(type) {
//         case GL_FLOAT: return sizeof(GLfloat);
//         case GL_DOUBLE: return sizeof(GLdouble);
//         case GL_INT: return sizeof(GLint);
//         case GL_UNSIGNED_INT: return sizeof(GLuint);
//         case GL_BYTE: return sizeof(GLbyte);
//         case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
//         default: return 0;
//     }
// }