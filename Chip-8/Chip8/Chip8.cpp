#include "Chip8.h"

#include "InputManager.h"
#include "Texture.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>

Chip8::Chip8(InputManager* inputManager)
{
    Init(inputManager);
}

void Chip8::Cycle(Texture* texture)
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

    UpdateScreen(texture);
}

void Chip8::LoadROM(std::string_view filePath)
{
    std::ifstream file(std::string(filePath), std::ios::binary | std::ios::ate);

    if (!file)
        return;

    const std::streamsize size = file.tellg();
    if (size <= 0)
        return;

    if (_cpu->GetStartAddress() + static_cast<size_t>(size) > _cpu->GetMemorySize())
        return;

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

void Chip8::Init(InputManager* inputManager)
{
    _cpu = new CPU();
    _cpu->Reset(_currentRom, _currRomSize);

    SetupKeyBindings(inputManager);
}

void Chip8::SingleCycle()
{
    _cpu->Fetch();

    _cpu->Decode();

    _cpu->Execute();

    _cpu->UpdateTimers();
}

void Chip8::UpdateScreen(Texture* texture)
{
    if (_cpu->GetWidth() != texture->GetWidth())
        texture->CreateEmpty(_cpu->GetWidth(), _cpu->GetHeight());

    texture->Update(_cpu->GetPixelData());
}

void Chip8::SetupKeyBindings(InputManager* input)
{
    input->BindKey(GLFW_KEY_X, EmulatorKey::Chip8_0);
    input->BindKey(GLFW_KEY_1, EmulatorKey::Chip8_1);
    input->BindKey(GLFW_KEY_2, EmulatorKey::Chip8_2);
    input->BindKey(GLFW_KEY_3, EmulatorKey::Chip8_3);

    input->BindKey(GLFW_KEY_Q, EmulatorKey::Chip8_4);
    input->BindKey(GLFW_KEY_W, EmulatorKey::Chip8_5);
    input->BindKey(GLFW_KEY_E, EmulatorKey::Chip8_6);
    input->BindKey(GLFW_KEY_A, EmulatorKey::Chip8_7);

    input->BindKey(GLFW_KEY_S, EmulatorKey::Chip8_8);
    input->BindKey(GLFW_KEY_D, EmulatorKey::Chip8_9);
    input->BindKey(GLFW_KEY_Z, EmulatorKey::Chip8_A);
    input->BindKey(GLFW_KEY_C, EmulatorKey::Chip8_B);

    input->BindKey(GLFW_KEY_4, EmulatorKey::Chip8_C);
    input->BindKey(GLFW_KEY_R, EmulatorKey::Chip8_D);
    input->BindKey(GLFW_KEY_F, EmulatorKey::Chip8_E);
    input->BindKey(GLFW_KEY_V, EmulatorKey::Chip8_F);
}
