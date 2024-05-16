#ifndef _INDEX_BUFFER_H
#define _INDEX_BUFFER_H

#include <glad/glad.h>

typedef struct IndexBuffer {
    GLuint id;
    GLuint count;
} IndexBuffer;

void IndexBuffer_init(IndexBuffer *const index_buffer, const GLuint *const data, const GLuint count);
void IndexBuffer_free(IndexBuffer *const index_buffer);
void IndexBuffer_bind(const IndexBuffer *const index_buffer);
void IndexBuffer_unbind(const IndexBuffer *const index_buffer);

#endif