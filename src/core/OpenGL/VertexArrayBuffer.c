#include <glad/glad.h>
#include "VertexArrayBuffer.h"

void VertexArrayBuffer_init(VertexArrayBuffer vab[static 1], const void *const data, const unsigned int size){
    Lock_init(&vab->lock);
    vab->index = 0;
    glGenBuffers(1, &vab->id);
    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

inline void VertexArrayBuffer_push(VertexArrayBuffer vab[static 1], const void *const data, const unsigned int size) {
    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
    glBufferSubData(GL_ARRAY_BUFFER, (long long int)vab->index, size, data);
    vab->index += (size_t)size;
}

inline void VertexArrayBuffer_set(VertexArrayBuffer vab[static 1], const unsigned int index, const void *const data, const unsigned int size) {
    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
    glBufferSubData(GL_ARRAY_BUFFER, (long long int)index, size, data); 
}

inline void VertexArrayBuffer_free(VertexArrayBuffer vab[static 1]) {
    Lock_free(&vab->lock);
    glDeleteBuffers(1, &vab->id);
}

inline void VertexArrayBuffer_bind(const VertexArrayBuffer vab[static 1]) {
    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
}

inline void VertexArrayBuffer_unbind(const VertexArrayBuffer vab[static 1]) {
    (void)vab;
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}