#pragma once

class Window;
class Texture;
class Chip8;
class DebugWindow;

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
    Texture* _screenTexture = nullptr;
    DebugWindow* _debugWindow = nullptr;
};