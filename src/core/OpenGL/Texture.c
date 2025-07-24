#include <assert.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Texture.h"
#include "../Util/util.h"

const char OB_TEXTURE_UNIFORMS[32][10] = {
    "texture00",
    "texture01",
    "texture02",
    "texture03",
    "texture04",
    "texture05",
    "texture06",
    "texture07",
    "texture08",
    "texture09",
    "texture10",
    "texture11",
    "texture12",
    "texture13",
    "texture14",
    "texture15",
    "texture16",
    "texture17",
    "texture18",
    "texture19",
    "texture20",
    "texture21",
    "texture22",
    "texture23",
    "texture24",
    "texture25",
    "texture26",
    "texture27",
    "texture28",
    "texture29",
    "texture30",
    "texture31",
};

extern int OB_MAX_TEXTURE_IMAGE_UNITS;
extern int OB_MAX_COMBINED_TEXTURE_IMAGE_UNITS;

static unsigned char DEFAULT_TEXTURE[4] = {0, 0, 0, 255}; //Opaque black

static void OB_generate_texture(struct OB_Texture *texture) {
    assert(texture != NULL);

    GLenum format;
    if(texture->channels == 3) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        format = GL_RGB;
    } else if(texture->channels == 4) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        format = GL_RGBA;
    } else {
        return;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void OB_Texture_init(struct OB_Texture *texture, const char *name, const char *path) {
    assert(texture != NULL);
    assert(name != NULL);
    assert(path != NULL);

    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

    if(data == NULL) {
        fprintf(stderr, "Couldn't load texture at %s\n", path);
        OB_Texture_color(texture, NULL, NULL);
        return;
    }

    texture->channels = channels;
    texture->width = width;
    texture->height = height;
    texture->data = data;
    texture->name = name;
    OB_generate_texture(texture);
}

void OB_Texture_free(struct OB_Texture *texture) {
    assert(texture != NULL);

    if(texture->data != DEFAULT_TEXTURE) {
        stbi_image_free(texture->data);
    }
    memset(texture, 0, sizeof(*texture));
    texture->data = NULL;
}

void OB_Texture_use(const struct OB_Texture *texture, unsigned slot) {
    assert(texture != NULL);

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void OB_Texture_color(struct OB_Texture *texture, const char* name, const struct OB_Color *color) {
    assert(texture != NULL);
    assert(name != NULL);
    assert(color != NULL);

    texture->channels = 4;
    texture->width = 1;
    texture->height = 1;
    texture->name = NULL ? "default" : name;

    if(color == NULL) {
        texture->data = DEFAULT_TEXTURE;
        return;
    }

    const size_t size = 4 * sizeof(unsigned char);
    texture->data = (unsigned char*)malloc(size);

    if(texture->data == NULL) {
        texture->data = DEFAULT_TEXTURE;
        return;
    }

    memcpy(texture->data, (const unsigned char*)color, size);
    OB_generate_texture(texture);
}