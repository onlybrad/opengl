#include <stdio.h>
#include <glad/glad.h>
#include "util.h"
#include "stdlib.h"
#include "string.h"

void GLClearError(void) {
    while(glGetError() != GL_NO_ERROR);
}

void GLCheckError(void) {
    GLenum error;
    while((error = glGetError())) {
        printf("GL Error: %u", error);
    }
}

char *file_get_contents(size_t *const length, const char *const path) {
    FILE *const file = fopen(path, "rb");

    if (!file) {
        perror(NULL);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *length = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);
    char *const buffer = malloc(*length);
    fread(buffer, 1, *length, file);
    fclose(file);
    
    return buffer;
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