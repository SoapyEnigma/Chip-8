#pragma once

#include "Util/Types.h"

struct GLFWwindow;

class Window
{
public:
    Window();
    ~Window();

    void Clear();
    void Render();

    bool ShouldClose();
    void FlagToClose();

    GLFWwindow* GetHandle() { return _window; }
    const i32 GetWidth() const { return _width; }
    const i32 GetHeight() const { return _height; }

private:
    void Init();

private:
    GLFWwindow* _window = nullptr;
    i32 _width = 1280;
    i32 _height = 720;
};