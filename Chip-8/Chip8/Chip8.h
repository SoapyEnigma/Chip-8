#pragma once

#include "CPU.h"

#include <string_view>

class Chip8
{
public:
    Chip8();
    ~Chip8() { delete _cpu; _cpu = nullptr; }

    void Cycle();
    void LoadROM(std::string_view filePath);
    void Reset();

    const CPU* GetCPU() const { return _cpu; }
    CPU* GetCPU() { return _cpu; } // Added for glfw key callbacks

    const size_t GetROMSize() const { return _currRomSize; }

    void SetPaused(bool p) { _paused = p; }
    void TogglePaused() { _paused = !_paused; }
    const bool IsPaused() const { return _paused; }
    void StepOnce() { _doStep = true; }
    void SetCyclesPerFrame(i32 n) { _cyclesPerFrame = std::max(1, n); }
    i32  GetCyclesPerFrame() const { return _cyclesPerFrame; }

private:
    void Init();
    void SingleCycle();

private:
    CPU* _cpu = nullptr;
    std::vector<char> _currentRom{};
    size_t _currRomSize = 0;

    bool _paused = true;
    bool _doStep = false;
    i32  _cyclesPerFrame = 10;
};