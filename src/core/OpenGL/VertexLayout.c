#include "VertexLayout.h"

extern GLint MAX_VERTEX_ATTRIBS;

void VertexLayout_init(VertexLayout *const layout) {
    layout->attributes_index = 0;
    layout->stride = 0;
}

GLboolean VertexLayout_push(VertexLayout *const layout, const VertexAttribute *const attribute) {
    if(layout->attributes_index == (GLuint)MAX_VERTEX_ATTRIBS) {
        return GL_FALSE;
    }

    if(attribute->count <= 4) {
        layout->attributes[layout->attributes_index++] = *attribute;
        layout->stride += (GLsizei)attribute->count * (GLsizei)sizeof(GLfloat);
    } else {
        for(unsigned int i=0u; i < attribute->count / 4; i++) {
            layout->attributes[layout->attributes_index++] = (VertexAttribute) {
                .normalized = attribute->normalized,
                .count = 4
            };
            layout->stride += 4 * (GLsizei)sizeof(GLfloat);
        }
        
        GLuint count = attribute->count % 4;
        if(count > 0) {
            layout->attributes[layout->attributes_index++] = (VertexAttribute) {
                .normalized = attribute->normalized,
                .count = count
            };
            layout->stride += (GLsizei)count * (GLsizei)sizeof(GLfloat);
        }
    }

    return GL_TRUE;
}