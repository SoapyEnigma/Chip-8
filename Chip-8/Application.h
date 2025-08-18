#pragma once

#include "Window.h"
#include "Chip8.h"
#include "Shader.h"
#include "Texture.h"

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void Init();
    void Update();
    void Render();

private:
    Window* _window = nullptr;
    Chip8* _chip = nullptr;
    Shader* _screen = nullptr;
    Texture* _screenTexture = nullptr;
};

