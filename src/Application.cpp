#include <cassert>

#include "glad/glad.h"
#include "glfw3.h"
#include "fmt/format.h"
#include "stb_image.h"

#include "Image.hpp"
#include "ImageRenderer.hpp"
#include "ResourceManager.hpp"

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
    m_CleanUp();

    assert(g_application == this);
    g_application = nullptr;
}

void Application::m_CleanUp()
{
    ResourceManager::Clear();
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
        fmt::print(stderr, "error: {}\n", e.what());
        return 1;
    }

    ResourceManager::Init(&m_window);
    ResourceManager::LoadShader("image");

    ImageRenderer::Init();
    return 0;
}

int Application::m_Main()
{
    int frame = 1;

    Image img({0, 0}, {100, 200}, "test.jpg");
    //Canvas canvas({m_window.Width(), m_window.Height()});
    while (!m_window.ShouldClose()) {
        m_window.PollEvents();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        /*if (m_window.MouseButtonPressed().lmb) {
            auto mPos = m_window.CursorPos();
            canvas.setPixel(mPos / canvas.Size(), {1, 0, 0});
            img.SetTexture(canvas.GetTexture());
        }*/
        img.Draw();

        m_window.SwapBuffers();
    }

    return 0;
}