#include <assert.h>
#include <stdlib.h>
#include "vertexlayout.h"

extern int OB_MAX_VERTEX_ATTRIBS;

void OB_VertexLayout_init(struct OB_VertexLayout *layout) {
    assert(layout != NULL);

    layout->attributes_index = 0u;
    layout->stride = 0;
}

bool OB_VertexLayout_push(struct OB_VertexLayout *layout, const struct OB_VertexAttribute attribute) {
    assert(layout != NULL);

    if(layout->attributes_index == (unsigned)OB_MAX_VERTEX_ATTRIBS) {
        return false;
    }

    if(attribute.count <= 4u) {
        layout->attributes[layout->attributes_index++] = attribute;
        layout->stride += (int)attribute.count * (int)sizeof(float);
    } else {
        for(unsigned i = 0u; i < attribute.count / 4; i++) {
            layout->attributes[layout->attributes_index].normalized = attribute.normalized;
            layout->attributes[layout->attributes_index].count = 4u;
            layout->attributes_index++;
            layout->stride += 4 * (int)sizeof(float);
        }
        
        unsigned count = attribute.count % 4u;
        if(count > 0) {
            layout->attributes[layout->attributes_index].normalized = attribute.normalized;
            layout->attributes[layout->attributes_index].count = count;
            layout->attributes_index++;
            layout->stride += (int)count * (int)sizeof(float);
        }
    }

    return true;
}