#pragma once

#include <string>

#include "glad/glad.h"
#include "glfw3.h"

class Window 
{
public:
    Window();
    ~Window();
    void Init(int screenWidth, int screenHeight, const std::string& title);
    int ShouldClose();
    void Close();
    std::string Title();
    void SetTitle(const std::string& newTitle);
    void SwapBuffers();
    void PollEvents();

    void KeyCallback(int key, int scancode, int action, int mode);
    void FrameBufferSizeCallback(int width, int height);

private:
    int m_width = 0, m_height = 0;
    std::string m_title;

    GLFWwindow* m_window = nullptr;

    static void m_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void m_FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void m_ErrorCallback(int code, const char* msg);
};