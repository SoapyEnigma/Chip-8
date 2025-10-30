#pragma once

#include <Util/Types.h>

class Texture
{
public:
    Texture();
    ~Texture();

    void CreateEmpty(i32 width, i32 height);

    void Update(const u32* pixels) const;

    void Bind(u32 unit = 0) const;

    const u32 ID() const { return _id; }

    const i32 GetWidth() const { return _w; }

private:
    u32 _id;
    i32 _w{};
    i32 _h{};
};