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

int Application::m_Init()
{
    return 0;
}

int Application::m_Main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);  // Multisampling for smooth resize
    GLFWwindow* window = glfwCreateWindow(600, 600, "Rock Paper Scissors Automata", 0, 0);
    if (!window) std::cerr << "Your GPU doesn't support OpenGL 3.3 context! Abort.\n";
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    	puts("Failed to initialize GLAD.");
    	return -1;
  	}

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

    int frame = 1;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << "Key: " << key << " " << GLFW_KEY_ESCAPE << std::endl;
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        std::cout << "ESC pressed.\n";
    }
}


void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}