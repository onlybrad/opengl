#include "VertexLayout.h"

extern int MAX_VERTEX_ATTRIBS;

void VertexLayout_init(VertexLayout layout[static 1]) {
    layout->attributes_index = 0u;
    layout->stride = 0;
}

bool VertexLayout_push(VertexLayout layout[static 1], const VertexAttribute attribute) {
    if(layout->attributes_index == (unsigned int)MAX_VERTEX_ATTRIBS) {
        return false;
    }

    if(attribute.count <= 4u) {
        layout->attributes[layout->attributes_index++] = attribute;
        layout->stride += (int)attribute.count * (int)sizeof(float);
    } else {
        for(unsigned int i=0u; i < attribute.count / 4; i++) {
            layout->attributes[layout->attributes_index++] = (VertexAttribute) {
                .normalized = attribute.normalized,
                .count = 4u
            };
            layout->stride += 4 * (int)sizeof(float);
        }
        
        unsigned int count = attribute.count % 4u;
        if(count > 0) {
            layout->attributes[layout->attributes_index++] = (VertexAttribute) {
                .normalized = attribute.normalized,
                .count = count
            };
            layout->stride += (int)count * (int)sizeof(float);
        }
    }

    return true;
}