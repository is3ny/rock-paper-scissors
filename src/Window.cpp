#include <stdexcept>
#include "fmt/format.h"

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
    m_keyPressed.resize(GLFW_KEY_LAST + 1, ButtonState::RELEASE);
    m_mouseButtonStates.resize(GLFW_MOUSE_BUTTON_LAST + 1, ButtonState::RELEASE);

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
    glfwSetMouseButtonCallback(m_window, m_MouseButtonCallback);
    glfwSetCursorPosCallback(m_window, m_CursorPosCallback);

    double xPos, yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    m_cursorPos.x = xPos;
    m_cursorPos.y = yPos;
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
    // Replace any presses on holds before the poll
    // If the button was released, then anyway it will be set to Release
    // If the button was already pressed and nothing happened, it means the
    // button is Held.
    // The poll after this procedure will correctly mark new pressed key as
    // Pressed.
    if (m_checkKeyHold) {
        checkHold(m_keyPressed);
        m_checkKeyHold = false;
    }

    if (m_checkMouseButtonHold) {
        checkHold(m_mouseButtonStates);
        m_checkMouseButtonHold = false;
    }

    glfwPollEvents();
}

ButtonState Window::KeyPressed(int id)
{
    return m_keyPressed.at(id);
}

ButtonState Window::MouseButtonPressed(int id)
{
    return m_mouseButtonStates.at(id);
}

glm::vec2 Window::CursorPos()
{
    return m_cursorPos;
}

void Window::KeyCallback(int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS) {
        m_keyPressed[key] = ButtonState::PRESS;
        m_checkKeyHold = true;
    } else if (action == GLFW_RELEASE) {
        m_keyPressed[key] = ButtonState::RELEASE;
    }
}

void Window::MouseButtonCallback(int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        m_mouseButtonStates[button] = ButtonState::PRESS;
        m_checkMouseButtonHold = true;
    } else if (action == GLFW_RELEASE) {
        m_mouseButtonStates[button] = ButtonState::RELEASE;
    }
}

void Window::CursorPosCallback(double xpos, double ypos)
{
    m_cursorPos.x = xpos;
    m_cursorPos.y = ypos;
}

void Window::FrameBufferSizeCallback(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}


void Window::checkHold(std::vector<ButtonState>& states)
{
    for (auto& s : states) {
        if (s == ButtonState::PRESS)
            s = ButtonState::HOLD;
    }
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

void Window::m_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Window* wrapper = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    wrapper->MouseButtonCallback(button, action, mods);
}

void Window::m_CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Window* wrapper = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    wrapper->CursorPosCallback(xpos, ypos);
}

void Window::m_ErrorCallback(int code, const char* msg)
{
    fmt::print(stderr, "glfw error ({}): {}\n", code, msg);
}