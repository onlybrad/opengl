#include <assert.h>
#include <glad/glad.h>
#include "IndexBuffer.h"

void IndexBuffer_init(IndexBuffer *index_buffer, const unsigned *const data, const unsigned count) {
    assert(index_buffer != NULL);
    
    index_buffer->count = count;
    glGenBuffers(1, &index_buffer->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), data, GL_STATIC_DRAW);
}

void IndexBuffer_free(IndexBuffer *index_buffer) {
    assert(index_buffer != NULL);
    
    glDeleteBuffers(1, &index_buffer->id);
}

void IndexBuffer_bind(const IndexBuffer *index_buffer) {
    assert(index_buffer != NULL);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
}

void IndexBuffer_unbind(const IndexBuffer *index_buffer) {
    assert(index_buffer != NULL);
    
    (void)index_buffer;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}