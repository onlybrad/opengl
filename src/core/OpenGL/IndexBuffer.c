#include <glad/glad.h>
#include "IndexBuffer.h"

void IndexBuffer_init(IndexBuffer index_buffer[static 1], const unsigned int *const data, const unsigned int count) {
    index_buffer->count = count;
    glGenBuffers(1, &index_buffer->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer_free(IndexBuffer index_buffer[static 1]) {
    glDeleteBuffers(1, &index_buffer->id);
}

void IndexBuffer_bind(const IndexBuffer index_buffer[static 1]) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
}

void IndexBuffer_unbind(const IndexBuffer index_buffer[static 1]) {
    (void)index_buffer;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}