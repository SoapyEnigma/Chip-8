#pragma once

#include "Util/Types.h"

#include <unordered_map>
#include <unordered_set>

enum class SpecialKey
{
    NONE,
    ESCAPE,
    ENTER
};

enum class EmulatorKey : u16
{
    Chip8_0, Chip8_1, Chip8_2, Chip8_3,
    Chip8_4, Chip8_5, Chip8_6, Chip8_7,
    Chip8_8, Chip8_9, Chip8_A, Chip8_B,
    Chip8_C, Chip8_D, Chip8_E, Chip8_F,

    NES_A, NES_B, NES_Select, NES_Start,
    NES_Up, NES_Down, NES_Left, NES_Right,
};

struct GLFWwindow;
class InputManager
{
public:
    void AttachToWindow(GLFWwindow* window);

    void Update();
    void BindKey(i32 hostKey, EmulatorKey emuKey);
    bool IsKeyDown(EmulatorKey key) const;
    bool IsSpecialKeyPressed(SpecialKey key) const;

private:
    void KeyCallback(i32 key, i32 scancode, i32 action, i32 mods);

private:
    GLFWwindow* _window = nullptr;
    std::unordered_map<i32, EmulatorKey> _keyBindings;
    std::unordered_set<EmulatorKey> _pressedKeys;
    bool _escapePressed = false;
    bool _enterPressed = false;
};