#pragma once

#include <imgui.h>

class Window;
class Chip8;
class Texture;

class DebugWindow
{
public:
    DebugWindow(Window* window, Chip8* chip);
    ~DebugWindow();

    void Render(Texture* texture);

private:
    void Init();

    void DockSpace();
    void EmuSpace(Texture* texture);
    void DebugSpace();

    void DebugCPU();
    void DebugStack();
    void DebugDisassembly();
    void DebugMemory();
    void DebugKeypad();

private:
    Window* _window = nullptr;
    Chip8* _chip = nullptr;
    bool _firstLoop = true;
    ImVec2 _lastSize = { 0, 0 };
};

