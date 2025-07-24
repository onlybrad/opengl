#ifndef _INDEX_BUFFER_H
#define _INDEX_BUFFER_H

typedef struct IndexBuffer {
    unsigned id;
    unsigned count;
} IndexBuffer;

void IndexBuffer_init(IndexBuffer *index_buffer, const unsigned *const data, const unsigned count);
void IndexBuffer_free(IndexBuffer *index_buffer);
void IndexBuffer_bind(const IndexBuffer *index_buffer);
void IndexBuffer_unbind(const IndexBuffer *index_buffer);

#endif