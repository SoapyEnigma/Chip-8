#include "Application.h"

#include "Window.h"
#include "Texture.h"
#include "Chip8.h"
#include "DebugWindow.h"

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
    _chip = new Chip8();

    _window->SetUserPtr(_chip);

    _screenTexture = new Texture();
    _screenTexture->CreateEmpty(64, 32);

    _debugWindow = new DebugWindow(_window, _chip);
}

void Application::Update()
{
    _window->Clear();
    _chip->Cycle();
    _screenTexture->Update(_chip->GetCPU()->GetPixelData());
}

void Application::Render()
{
    _debugWindow->Render(_screenTexture);
    _window->Render();
}
