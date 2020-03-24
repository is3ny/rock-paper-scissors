#include <iostream>
#include <cassert>

#include "glad/glad.h"
#include "glfw3.h"

#include "Application.hpp"


Application* g_application;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

Application::Application()
{
    assert(!g_application);
    g_application = this;
}

Application::~Application()
{
    assert(g_application == this);
    g_application = nullptr;
}

void Application::SetCommandArgs(int count, char* args[])
{
    this->args = std::vector<std::string>(args, args + count);
}

int Application::Run()
{
    int error = Application::m_Init();
    if (error)
        return error;

    return m_Main();
}

// TODO: Add an enumeration with success / failure, instead of 0 and 1.
int Application::m_Init()
{
    try {
        m_window.Init(600, 600, "Hello!");
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

int Application::m_Main()
{
    int frame = 1;

    while (!m_window.ShouldClose()) {
        m_window.PollEvents();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        img.draw();

        m_window.SwapBuffers();
    }

    return 0;
}