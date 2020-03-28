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

    static void Shader(const std::string& name);
    static void Pixesls(const std::string& name);

private:
    static ObjectPool<Shader> m_shaderPool;
    static ObjectPool<PixelData> m_pixelsPool;

    // TODO: Find out if this OK. May break if Window gets destroyed.
    static Window* m_window;
};
