#include "Texture.h"

#include <glad/glad.h>

Texture::Texture()
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    if (_id)
        glDeleteTextures(1, &_id);
}

void Texture::CreateEmpty(i32 width, i32 height)
{
    _w = width;
    _h = height;

    glBindTexture(GL_TEXTURE_2D, _id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Update(const u32* pixels) const
{
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(u32 unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}