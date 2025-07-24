#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <stdbool.h>
#include "../Util/Color.h"

typedef struct Texture {
    unsigned id;
    unsigned char *data;
    const char *name;
    int width;
    int height;
    int channels;
} Texture;

void Texture_init(Texture *texture, const char *name, const char *path);
void Texture_free(Texture *texture);
void Texture_use(const Texture *texture, const unsigned slot);
bool Texture_color(Texture *texture, const char* name, const Color *color);

#endif