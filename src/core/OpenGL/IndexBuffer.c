#include "IndexBuffer.h"

void IndexBuffer_init(IndexBuffer *const index_buffer, const GLuint *const data, const GLuint count) {
    index_buffer->count = count;
    glGenBuffers(1, &index_buffer->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

void IndexBuffer_free(IndexBuffer *const index_buffer) {
    glDeleteBuffers(1, &index_buffer->id);
}

void IndexBuffer_bind(const IndexBuffer *const index_buffer) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
}

void IndexBuffer_unbind(const IndexBuffer *const index_buffer) {
    (void)index_buffer;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}