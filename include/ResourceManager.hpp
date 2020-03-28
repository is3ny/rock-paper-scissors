#pragma once

#include <string>
#include "ObjectPool.hpp"
#include "Shader.hpp"
#include "Window.hpp"

class ResourceManager
{
public:
    static void Init(const Window& window);

    static void LoadShader(const std::string& name);
    static void LoadPixels(const std::string& name);

    static void GetShader(const std::string& name);
    static void GetPixels(const std::string& name);

private:
    static ObjectPool<Shader, std::string> m_shaderPool;
    static ObjectPool<PixelData, std::string> m_pixelsPool;

    // TODO: Find out if this OK. May break if Window gets destroyed.
    static Window* m_window;
};
