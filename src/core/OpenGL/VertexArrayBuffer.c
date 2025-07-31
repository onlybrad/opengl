#include <string.h>
#include <assert.h>
#include <glad/glad.h>
#include "vertexarraybuffer.h"

void OB_VertexArrayBuffer_init(struct OB_VertexArrayBuffer *vab, const void *data, unsigned size){
    assert(vab != NULL);

    OB_Lock_init(&vab->lock);
    vab->index = 0;
    glGenBuffers(1, &vab->id);
    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void OB_VertexArrayBuffer_push(struct OB_VertexArrayBuffer *vab, const void *data, unsigned size) {
    assert(vab != NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
    glBufferSubData(GL_ARRAY_BUFFER, (long long)vab->index, size, data);
    vab->index += (size_t)size;
}

void OB_VertexArrayBuffer_set(struct OB_VertexArrayBuffer *vab, unsigned index, const void *data, unsigned size) {
    assert(vab != NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
    glBufferSubData(GL_ARRAY_BUFFER, (long long)index, size, data); 
}

void OB_VertexArrayBuffer_free(struct OB_VertexArrayBuffer *vab) {
    assert(vab != NULL);

    OB_Lock_free(&vab->lock);
    glDeleteBuffers(1, &vab->id);
    memset(vab, 0, sizeof(*vab));
}

void OB_VertexArrayBuffer_bind(const struct OB_VertexArrayBuffer *vab) {
    assert(vab != NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vab->id);
}

void OB_VertexArrayBuffer_unbind(const struct OB_VertexArrayBuffer *vab) {
    assert(vab != NULL);

    (void)vab;
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}