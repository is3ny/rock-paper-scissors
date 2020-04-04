#include <cassert>

#include "glad/glad.h"
#include "glfw3.h"
#include "fmt/format.h"
#include "fmt/printf.h"
#include "stb_image.h"

#include "Image.hpp"
#include "ImageRenderer.hpp"
#include "ResourceManager.hpp"
#include "Canvas.hpp"

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
    ResourceManager::LoadShader("color_fill");
    ResourceManager::LoadShader("draw_pixel");
    ResourceManager::LoadShader("draw_line");
    ResourceManager::LoadShader("resize_canvas");
    ResourceManager::LoadShader("canvas_out");
    ResourceManager::LoadShader("step");
    

    stbi_set_flip_vertically_on_load(true);
    ImageRenderer::Init();
    return 0;
}

// This function is a mess and right now is used only for testing purposes
int Application::m_Main()
{
    int frame = 1;

    
    //Canvas canvas({m_window.Width(), m_window.Height()});
    //Canvas canvas({600, 600});
    Canvas canvas({200, 200});
    canvas.Resize({1000, 1000});
    canvas.SetPalette({{0,0,0}, {0.1, 0, 0}, {0.2, 0, 0}, {0.3, 0, 0}});
    int pickedColor = 1;

    Image img({0, 0}, {500, 600}, canvas.GetTexture());

    glm::vec2 prevPos = m_window.CursorPos();
    glm::ivec2 prevWinSize = m_window.Size();

    Texture canvasOutput;

    double t = glfwGetTime();
    double dt = t;
    int frameCount = 0;
    int stepCount = 0;
    bool genTex = 0;
    std::string statusLine;
    while (!m_window.ShouldClose()) {
        m_window.PollEvents();

        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (m_window.MouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::PRESS) {
            prevPos = m_window.CursorPos();
        } else if (m_window.MouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) == ButtonState::HOLD) {
            auto mPos = m_window.CursorPos();
            canvas.DrawLine(prevPos, mPos, {pickedColor, 10}, m_window.Size());
            img.SetTexture(canvas.GetTexture());
            // canvas.GenerateTexture(img.GetTexture());
            prevPos = mPos;
        }

        if (prevWinSize != m_window.Size()) {
            //canvas.Resize(canvas.Size() + (glm::vec2)(m_window.Size() - prevWinSize));
            fmt::print(stderr, "\rsize: {} x {}", canvas.Size().x, canvas.Size().y);
            prevWinSize = m_window.Size();
        }
        if (m_window.KeyPressed(GLFW_KEY_LEFT) == ButtonState::HOLD) {
            canvas.Resize({canvas.Size().x + 1, canvas.Size().y});
            // canvas.GenerateTexture(img.GetTexture());
            img.SetTexture(canvas.GetTexture());
            fmt::print(stderr, "\rsize: {} x {}", canvas.Size().x, canvas.Size().y);
        }

        if (m_window.KeyPressed(GLFW_KEY_RIGHT) == ButtonState::HOLD) {
            canvas.Resize({canvas.Size().x - 1, canvas.Size().y});
            // canvas.GenerateTexture(img.GetTexture());
            img.SetTexture(canvas.GetTexture());
            fmt::print(stderr, "\rsize: {} x {}", canvas.Size().x, canvas.Size().y);
        }

        if (m_window.KeyPressed(GLFW_KEY_DOWN) == ButtonState::HOLD) {
            canvas.Resize({canvas.Size().x, canvas.Size().y - 1});
            // canvas.GenerateTexture(img.GetTexture());
            img.SetTexture(canvas.GetTexture());
            fmt::print(stderr, "\rsize: {} x {}", canvas.Size().x, canvas.Size().y);
        }

        if (m_window.KeyPressed(GLFW_KEY_UP) == ButtonState::HOLD) {
            canvas.Resize({canvas.Size().x, canvas.Size().y + 1});
            // canvas.GenerateTexture(img.GetTexture());
            img.SetTexture(canvas.GetTexture());
            fmt::print(stderr, "\rsize: {} x {}", canvas.Size().x, canvas.Size().y);
        }
        if (m_window.KeyPressed(GLFW_KEY_Q))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (m_window.KeyPressed(GLFW_KEY_W))
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (m_window.KeyPressed(GLFW_KEY_R) == ButtonState::PRESS) {
            genTex ^= 1;
        }

        if (m_window.KeyPressed(GLFW_KEY_1) == ButtonState::PRESS)
            pickedColor = 0;
        if (m_window.KeyPressed(GLFW_KEY_2) == ButtonState::PRESS)
            pickedColor = 1;
        if (m_window.KeyPressed(GLFW_KEY_3) == ButtonState::PRESS)
            pickedColor = 2;
        if (m_window.KeyPressed(GLFW_KEY_4) == ButtonState::PRESS)
            pickedColor = 3;

        if (m_window.KeyPressed(GLFW_KEY_S) == ButtonState::HOLD) {
            float takes = 0;
            double startTime = glfwGetTime();
            while (glfwGetTime() - startTime < 1.0 / 80) {
                canvas.Step();
                stepCount++;
                if (takes < 1e-6)
                    takes = glfwGetTime() - startTime;
            }
            img.SetTexture(canvas.GetTexture());
        } else {
            canvas.Step();
            stepCount++;
        }

        if (genTex) {
            canvas.GenerateTexture(canvasOutput);
            img.SetTexture(canvasOutput);
        } else {
            img.SetTexture(canvas.GetTexture());
        }

        img.SetSize(m_window.Size());
        img.Draw();

        m_window.SwapBuffers();
        frameCount++;

        double nt = glfwGetTime();
        if (nt - t >= 1) {
            fmt::print(stderr, "\r{}", std::string(statusLine.size(), ' '));
            statusLine = fmt::format("\r{} fps | {} steps", frameCount, stepCount);
            fmt::print(stderr, "\r{}", statusLine);
            m_window.SetTitle(fmt::format("{} fps | {} steps", frameCount, stepCount));
            t = nt;
            frameCount = 0;
            stepCount = 0;
        }
        dt = glfwGetTime();
    }

    return 0;
}