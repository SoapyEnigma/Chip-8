#pragma once

#include "Types.h"

#include <glad/glad.h>

#include <array>

class Texture
{
public:
    Texture()
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

    ~Texture()
    {
        if (_id)
            glDeleteTextures(1, &_id);
    }

    void CreateEmpty(i32 width, i32 height)
    {
        _w = width;
        _h = height;

        glBindTexture(GL_TEXTURE_2D, _id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Update(const u32* pixels) const
    {
        glBindTexture(GL_TEXTURE_2D, _id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _w, _h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Bind(u32 unit = 0) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    u32 ID() const { return _id; }

private:
    u32 _id;
    i32 _w{};
    i32 _h{};
};