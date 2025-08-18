#pragma once

#include "Types.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Window
{
public:
    Window();
    ~Window();

    void Clear();
    void Render();

    bool ShouldClose() { return glfwWindowShouldClose(_window); }

    void* GetUserPtr() const { return glfwGetWindowUserPointer(_window); }
    void SetUserPtr(void* p) { glfwSetWindowUserPointer(_window, p); }

private:
    void Init();

private:
    GLFWwindow* _window = nullptr;
    i32 _width = 720;
    i32 _height = 480;
};