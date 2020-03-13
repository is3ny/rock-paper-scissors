#include <iostream>
#include <cassert>

#include "Application.hpp"


Application* g_application;

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
    std::cout << "Hello!\n";
    return 0;
}

