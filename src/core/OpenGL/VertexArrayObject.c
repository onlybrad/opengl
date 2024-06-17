#include <glad/glad.h>
#include <stdlib.h>
#include "VertexArrayObject.h"
#include "../Util/util.h"

void VertexArrayObject_init(VertexArrayObject vao[static 1]) {
    glGenVertexArrays(1, &vao->id);
}

void VertexArrayObject_free(VertexArrayObject vao[static 1]) {
    glDeleteVertexArrays(1, &vao->id);
}

void VertexArrayObject_add_buffer(VertexArrayObject vao[static 1], const VertexArrayBuffer *const vab, const IndexBuffer *const ib, const VertexLayout *const layout) {
    VertexArrayObject_bind(vao);
    VertexArrayBuffer_bind(vab);
    
    if(ib != NULL) {
        IndexBuffer_bind(ib);
    }

    int offset = 0;

    for(unsigned int i=0; i<layout->attributes_index; i++) {
        const VertexAttribute *const attribute = layout->attributes + i;
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, (int)attribute->count, GL_FLOAT, attribute->normalized, layout->stride, INT_TO_PTR(offset));
        offset += (int)attribute->count * (int)sizeof(GLfloat);
    }
}

void VertexArrayObject_bind(const VertexArrayObject vao[static 1]) {
    glBindVertexArray(vao->id);
}

void VertexArrayObject_unbind(const VertexArrayObject vao[static 1]) {
    (void)vao;
    glBindVertexArray(0u);
}
