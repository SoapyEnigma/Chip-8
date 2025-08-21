#include "Chip8.h"

#include <filesystem>
#include <fstream>

Chip8::Chip8()
{
    Init();
}

void Chip8::Cycle()
{
    if (_paused)
    {
        if (_doStep)
        {
            SingleCycle();
            _doStep = false;
        }
    }
    else
    {
        for (i32 i = 0; i < _cyclesPerFrame; i++)
            SingleCycle();
    }
}

void Chip8::LoadROM(std::string_view filePath)
{
    std::ifstream file(std::string(filePath), std::ios::binary | std::ios::ate);

    if (!file)
        return; // Fails

    const std::streamsize size = file.tellg();
    if (size <= 0)
        return; // Fails
    if (_cpu->GetStartAddress() + static_cast<size_t>(size) > _cpu->GetMemorySize())
        return; // Fails

    _currentRom.resize(size);
    _currRomSize = size;

    file.seekg(0, std::ios::beg);
    file.read(_currentRom.data(), size);

    Reset();
}

void Chip8::Reset()
{
    _cpu->Reset(_currentRom, _currRomSize);
}

void Chip8::Init()
{
    _cpu = new CPU();
    _cpu->Reset(_currentRom, _currRomSize);
}

void Chip8::SingleCycle()
{
    _cpu->Fetch();

    _cpu->Decode();

    _cpu->Execute();

    _cpu->UpdateTimers();
}
