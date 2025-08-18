#include "Application.h"

Application::Application()
{
    Init();
}

Application::~Application()
{
    delete _screenTexture;
    _screenTexture = nullptr;

    delete _screen;
    _screen = nullptr;

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
    _chip->LoadROM("../Roms/15 Puzzle [Roger Ivie].ch8");

    _screen = new Shader();
    _screenTexture = new Texture();
    _screenTexture->CreateEmpty(64, 32);
}

void Application::Update()
{
    _window->Clear();
    _chip->Cycle();
    _screenTexture->Update(_chip->GetPixelData());
}

void Application::Render()
{
    _screenTexture->Bind();
    _screen->Draw();
    _window->Render();
}
