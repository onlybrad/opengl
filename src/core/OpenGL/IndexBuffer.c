#include <assert.h>
#include <string.h>
#include <glad/glad.h>
#include "indexbuffer.h"

void OB_IndexBuffer_init(struct OB_IndexBuffer *index_buffer, unsigned *data, unsigned count) {
    assert(index_buffer != NULL);
    
    index_buffer->count = count;
    glGenBuffers(1, &index_buffer->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), data, GL_STATIC_DRAW);
}

void OB_IndexBuffer_free(struct OB_IndexBuffer *index_buffer) {
    assert(index_buffer != NULL);
    
    glDeleteBuffers(1, &index_buffer->id);
    memset(index_buffer, 0, sizeof(*index_buffer));
}

void OB_IndexBuffer_bind(const struct OB_IndexBuffer *index_buffer) {
    assert(index_buffer != NULL);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->id);
}

void OB_IndexBuffer_unbind(const struct OB_IndexBuffer *index_buffer) {
    assert(index_buffer != NULL);
    
    (void)index_buffer;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}