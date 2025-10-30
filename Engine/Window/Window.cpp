#include "Window.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Window::Window()
{
    Init();
}

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::Clear()
{
    glfwGetFramebufferSize(_window, &_width, &_height);
    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Render()
{
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void Window::Init()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, "Chip-8", nullptr, nullptr);

    glfwMakeContextCurrent(_window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    glfwSwapInterval(1);
}