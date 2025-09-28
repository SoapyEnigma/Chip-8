#pragma once

#include "CPU.h"

#include <algorithm>
#include <string_view>

class InputManager;
class Texture;

class Chip8
{
public:
    Chip8(InputManager* inputManager);
    ~Chip8() { delete _cpu; _cpu = nullptr; }

    void Cycle(Texture* texture);
    void LoadROM(std::string_view filePath);
    void Reset();

    const CPU* GetCPU() const { return _cpu; }
    CPU* GetCPU() { return _cpu; }

    const size_t GetROMSize() const { return _currRomSize; }

    void SetPaused(bool p) { _paused = p; }
    void TogglePaused() { _paused = !_paused; }
    const bool IsPaused() const { return _paused; }
    void StepOnce() { _doStep = true; }
    void SetCyclesPerFrame(i32 n) { _cyclesPerFrame = (std::max)(1, n); }
    i32  GetCyclesPerFrame() const { return _cyclesPerFrame; }

private:
    void Init(InputManager* inputManager);
    void SingleCycle();
    void UpdateScreen(Texture* texture);
    void SetupKeyBindings(InputManager* input);

private:
    CPU* _cpu = nullptr;
    std::vector<char> _currentRom{};
    size_t _currRomSize = 0;

    bool _paused = true;
    bool _doStep = false;
    i32  _cyclesPerFrame = 10;
};