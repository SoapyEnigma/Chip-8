#include "Application.h"

#include "Chip8/Chip8.h"
#include "UI/DebugWindow.h"

#include <Engine/Input/InputManager.h>
#include <Engine/Renderer/Texture.h>
#include <Engine/Window/Window.h>

#define GLFW_INCLUDE_NONE
#include <Vendor/glfw/include/GLFW/glfw3.h>

Application::Application()
{
    Init();
}

Application::~Application()
{
    delete _debugWindow;
    _debugWindow = nullptr;

    delete _screenTexture;
    _screenTexture = nullptr;

    delete _chip;
    _chip = nullptr;

    delete _window;
    _window = nullptr;
}

void Application::Run()
{
    while (!_window->ShouldClose())
    {
        Update();
        Render();
    }
}

void Application::Init()
{
    _window = new Window();

    _inputManager = new InputManager();
    _inputManager->AttachToWindow(_window->GetHandle());

    _chip = new Chip8(_inputManager);

    _screenTexture = new Texture();
    _screenTexture->CreateEmpty(64, 32);

    _debugWindow = new DebugWindow(_window, _chip);
}

void Application::Update()
{
    _window->Clear();

    _inputManager->Update();

    HandleInput();

    _chip->Cycle(_screenTexture);
}

void Application::Render()
{
    _debugWindow->Render(_screenTexture);
    _window->Render();
}

void Application::HandleInput()
{
    for (i32 i = 0; i < 16; i++) // Chip 8 Specific
    {
        auto key = static_cast<EmulatorKey>(static_cast<i32>(EmulatorKey::Chip8_0) + i);
        if (_inputManager->IsKeyDown(key))
            _chip->GetCPU()->KeyDown(i);
        else
            _chip->GetCPU()->KeyUp(i);
    }

    if (_inputManager->IsSpecialKeyPressed(SpecialKey::ENTER))
        _chip->Reset();

    if (_inputManager->IsSpecialKeyPressed(SpecialKey::ESCAPE))
        glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
}
