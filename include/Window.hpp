#pragma once

#include <string>

#include "glad/glad.h"
#include "glfw3.h"
#include "glm/glm.hpp"

enum ButtonState {
    // HOLD if the key was pressed for at least 2 polls
    RELEASE, PRESS, HOLD
};

class Window 
{
public:
    Window();
    ~Window();
    void Init(int screenWidth, int screenHeight, const std::string& title);
    int ShouldClose();
    void Close();
    std::string Title();
    /// In pixels
    int Width();
    /// In pixels
    int Height();
    void SetTitle(const std::string& newTitle);
    void SwapBuffers();
    void PollEvents();

    // Return the number of polls for which the key was pressed continuously
    ButtonState KeyPressed(int id);
    ButtonState MouseButtonPressed(int id);

    glm::vec2 CursorPos();

    void KeyCallback(int key, int scancode, int action, int mode);
    void FrameBufferSizeCallback(int width, int height);
    void MouseButtonCallback(int button, int action, int mods);
    void CursorPosCallback(double xpos, double ypos);

private:
    int m_width = 0, m_height = 0;
    std::string m_title;

    // Callbacks will set these to true if any key was pressed
    // On the next poll the states will be examined again if a flag is set
    // Optimization: event polling won't check all keys each invokation.
    bool m_checkKeyHold = false;
    bool m_checkMouseButtonHold = false;
    std::vector<ButtonState> m_keyPressed;
    std::vector<ButtonState> m_mouseButtonStates;
    glm::vec2 m_cursorPos;

    GLFWwindow* m_window = nullptr;

    void checkHold(std::vector<ButtonState>& states);

    static void m_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void m_FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void m_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void m_CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void m_ErrorCallback(int code, const char* msg);
};