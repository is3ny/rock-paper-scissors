#pragma once

#include <vector>
#include <string>

extern class Application* g_application;

class Application
{
public:
    Application();
    ~Application();

    void SetCommandArgs(int count, char* args[]);

    int Run();

private:
    std::vector<std::string> args;

    int m_Init();
    int m_Main();

};
