#ifndef OB_VERTEX_ARRAY_BUFFER_H
#define OB_VERTEX_ARRAY_BUFFER_H

#include <stdlib.h>
#include "../Thread/Thread.h"

struct OB_VertexArrayBuffer {
    unsigned id;
    size_t index;
    struct OB_Lock lock;
};

void OB_VertexArrayBuffer_init(struct OB_VertexArrayBuffer *vab, const void *data, unsigned size);
void OB_VertexArrayBuffer_push(struct OB_VertexArrayBuffer *vab, const void *data, unsigned size);
void OB_VertexArrayBuffer_set(struct OB_VertexArrayBuffer *vab, unsigned index, const void *data, unsigned size);
void OB_VertexArrayBuffer_free(struct OB_VertexArrayBuffer *vab);
void OB_VertexArrayBuffer_bind(const struct OB_VertexArrayBuffer *vab);
void OB_VertexArrayBuffer_unbind(const struct OB_VertexArrayBuffer *vab);

#endif