#ifndef OB_VERTEX_LAYOUT
#define OB_VERTEX_LAYOUT

#include <stdbool.h>

struct OB_VertexAttribute {
    unsigned count;
    bool normalized;
};

struct OB_VertexLayout {
    struct OB_VertexAttribute attributes[32];
    unsigned attributes_index;
    int stride;
};

void OB_VertexLayout_init(struct OB_VertexLayout *layout);
bool OB_VertexLayout_push(struct OB_VertexLayout *layout, const struct OB_VertexAttribute attribute);

#endif