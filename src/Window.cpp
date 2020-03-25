#include <stdexcept>
#include <iostream>

#include "Window.hpp"


static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

Window::Window()
{
    glfwSetErrorCallback(m_ErrorCallback);
    glfwInit();
}

Window::~Window()
{
    glfwTerminate();
}

// TODO: Add a WindowOptions class to handle hints in a general way and to be
//       able to output the config of the context, so that the user could be 
//       spared of not knowing what a possible issue could be.
void Window::Init(int screenWidth, int screenHeight, const std::string& title)
{
    m_title = title;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);  // Multisampling for smooth resize

    m_window = glfwCreateWindow(screenWidth, screenHeight, m_title.data(), 0, 0);
    if (!m_window) 
        throw std::runtime_error("Failed to initalize OpenGL context.");

    glfwMakeContextCurrent(m_window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD.");

    // Due to different DPI on different monitors, the actual _pixel_
    // dimensions may differ from the specified.
    glfwGetFramebufferSize(m_window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);

    // Store a pointer to this class, to call member callback functions from
    // within the static callbacks
    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

    glfwSetKeyCallback(m_window, m_KeyCallback);
    glfwSetFramebufferSizeCallback(m_window, m_FrameBufferSizeCallback);
}

int Window::ShouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void Window::Close()
{
    return glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

std::string Window::Title()
{
    return m_title;
}

int Window::Width()
{
    return m_width;
}

int Window::Height()
{
    return m_height;
}

void Window::SetTitle(const std::string& newTitle)
{
    m_title = newTitle;
    glfwSetWindowTitle(m_window, m_title.data());
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}


void Window::KeyCallback(int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE) {
        Close();
    }
}

void Window::FrameBufferSizeCallback(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}


void Window::m_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    Window* wrapper = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    wrapper->KeyCallback(key, scancode, action, mode);
}

void Window::m_FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Window* wrapper = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    wrapper->FrameBufferSizeCallback(width, height);
}

void Window::m_ErrorCallback(int code, const char* msg)
{
    std::cerr << "glfw error (" << code << "): " << msg << '\n';
}