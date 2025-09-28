#include "InputManager.h"

void InputManager::AttachToWindow(GLFWwindow* window)
{
    _window = window;

    glfwSetWindowUserPointer(_window, this);
    glfwSetKeyCallback(_window, [](GLFWwindow* win, i32 key, i32 scancode, i32 action, i32 mods)
        {
            InputManager* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
            if (manager)
                manager->KeyCallback(key, scancode, action, mods);
        });
}

void InputManager::Update()
{
    _pressedKeys.clear();

    for (auto& [hostKey, emuKey] : _keyBindings)
    {
        if (glfwGetKey(_window, hostKey) == GLFW_PRESS)
            _pressedKeys.insert(emuKey);
    }
}

void InputManager::BindKey(i32 hostKey, EmulatorKey emuKey)
{
    _keyBindings[hostKey] = emuKey;
}

bool InputManager::IsKeyDown(EmulatorKey key) const
{
    return _pressedKeys.contains(key);
}

bool InputManager::IsSpecialKeyPressed(SpecialKey key) const
{
    switch (key)
    {
        case SpecialKey::ESCAPE: return _escapePressed;
        case SpecialKey::ENTER:  return _enterPressed;
    }

    return false;
}

void InputManager::KeyCallback(i32 key, i32 scancode, i32 action, i32 mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        _escapePressed = (action == GLFW_PRESS);
        return;
    }

    if (key == GLFW_KEY_ENTER)
    {
        _enterPressed = (action == GLFW_PRESS);
        return;
    }
}