#ifndef _INDEX_BUFFER_H
#define _INDEX_BUFFER_H

#include <glad/glad.h>

typedef struct IndexBuffer {
    unsigned int id;
    unsigned int count;
} IndexBuffer;

void IndexBuffer_init(IndexBuffer *const index_buffer, const unsigned int *const data, const unsigned int count);
void IndexBuffer_free(IndexBuffer *const index_buffer);
void IndexBuffer_bind(const IndexBuffer *const index_buffer);
void IndexBuffer_unbind(const IndexBuffer *const index_buffer);

#endif