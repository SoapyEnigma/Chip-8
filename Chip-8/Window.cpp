#include "Window.h"

#include "Chip8.h"

static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
static u8 MapGlfwKeyToChip8(i32 key);

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

void Window::Init()
{
    if (!glfwInit())
    {
        // Fails
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, "Chip-8", nullptr, nullptr);
    if (!_window)
    {
        // Fails
    }

    glfwSetKeyCallback(_window, KeyCallback);

    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // Fails
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    glfwSwapInterval(1);
}

static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);


    Chip8* chip = static_cast<Chip8*>(glfwGetWindowUserPointer(window));
    if (!chip)
        return;

    u8 hex = MapGlfwKeyToChip8(key);
    if (hex == 0xFF)
        return;

    if (action == GLFW_PRESS)
        chip->KeyDown(hex);
    else if (action == GLFW_RELEASE)
        chip->KeyUp(hex);
}


static u8 MapGlfwKeyToChip8(i32 key)
{
    switch (key)
    {
    case GLFW_KEY_1: return 0x1;
    case GLFW_KEY_2: return 0x2;
    case GLFW_KEY_3: return 0x3;
    case GLFW_KEY_4: return 0xC;

    case GLFW_KEY_Q: return 0x4;
    case GLFW_KEY_W: return 0x5;
    case GLFW_KEY_E: return 0x6;
    case GLFW_KEY_R: return 0xD;

    case GLFW_KEY_A: return 0x7;
    case GLFW_KEY_S: return 0x8;
    case GLFW_KEY_D: return 0x9;
    case GLFW_KEY_F: return 0xE;

    case GLFW_KEY_Z: return 0xA;
    case GLFW_KEY_X: return 0x0;
    case GLFW_KEY_C: return 0xB;
    case GLFW_KEY_V: return 0xF;

    default: return 0xFF;
    }
}
