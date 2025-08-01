#ifndef OB_INDEX_BUFFER_H
#define OB_INDEX_BUFFER_H

struct OB_IndexBuffer {
    unsigned id;
    unsigned count;
};

void OB_IndexBuffer_init(struct OB_IndexBuffer *index_buffer, const unsigned *data, unsigned count);
void OB_IndexBuffer_free(struct OB_IndexBuffer *index_buffer);
void OB_IndexBuffer_bind(const struct OB_IndexBuffer *index_buffer);
void OB_IndexBuffer_unbind(const struct OB_IndexBuffer *index_buffer);

#endif