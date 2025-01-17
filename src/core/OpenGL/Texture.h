#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "../Util/Color.h"

typedef struct Texture {
    unsigned int id;
    unsigned char *data;
    const char *name;
    int width;
    int height;
    int channels;
} Texture;

void Texture_init(Texture texture[static 1], const char *const name, const char path[static 1]);
void Texture_free(Texture texture[static 1]);
void Texture_use(const Texture texture[static 1], const unsigned int slot);
void Texture_color(Texture texture[static 1], const char* const name, Color *const color);

#endif