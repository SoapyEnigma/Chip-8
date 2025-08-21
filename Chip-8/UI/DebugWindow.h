#pragma once

#include "Types.h"

#include <imgui.h>

#include <filesystem>

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

    void ScanRoms();
    void RomPicker();

    void ToolBar();

private:
    Window* _window = nullptr;
    Chip8* _chip = nullptr;

    bool _firstLoop = true;
    ImVec2 _lastSize = { 0, 0 };

    std::filesystem::path _romDir;
    std::vector<std::filesystem::path> _roms;
    i32 _romIndex = -1;
};