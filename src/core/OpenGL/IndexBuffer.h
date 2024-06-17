#ifndef _INDEX_BUFFER_H
#define _INDEX_BUFFER_H

typedef struct IndexBuffer {
    unsigned int id;
    unsigned int count;
} IndexBuffer;

void IndexBuffer_init(IndexBuffer index_buffer[static 1], const unsigned int *const data, const unsigned int count);
void IndexBuffer_free(IndexBuffer index_buffer[static 1]);
void IndexBuffer_bind(const IndexBuffer index_buffer[static 1]);
void IndexBuffer_unbind(const IndexBuffer index_buffer[static 1]);

#endif