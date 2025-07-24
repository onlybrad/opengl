#ifndef OB_TEXTURE_H
#define OB_TEXTURE_H

#include <stdbool.h>
#include "../Util/Color.h"

struct OB_Texture {
    unsigned id;
    unsigned char *data;
    const char *name;
    int width;
    int height;
    int channels;
};

void OB_Texture_init(struct OB_Texture *texture, const char *name, const char *path);
void OB_Texture_free(struct OB_Texture *texture);
void OB_Texture_use(const struct OB_Texture *texture, unsigned slot);
void OB_Texture_color(struct OB_Texture *texture, const char* name, const struct OB_Color *color);

#endif