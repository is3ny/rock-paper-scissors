#pragma once

#include <vector>
#include <string>

#include "Window.hpp"

extern class Application* g_application;

class Application
{
public:
    Application();
    ~Application();

    void SetCommandArgs(int count, char* args[]);

    int Run();

private:
    // TODO: Rename to m_args
    std::vector<std::string> args; 
    Window m_window;

    int m_Init();
    int m_Main();
    void m_CleanUp();

};
