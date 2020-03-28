#pragma once

#include <string>
#include <map>

#include "PixelData.hpp"
#include "Shader.hpp"
#include "Window.hpp"

template<typename T, typename K>
using ObjectPool = std::map<T, K>;

class ResourceManager
{
public:
    static void Init(const Window* window);

    static void LoadShader(const std::string& name);
    static void LoadPixels(const std::string& name);

    static Shader& GetShader(const std::string& name);
    static PixelData& GetPixels(const std::string& name);

    static void Clear();

private:
    static ObjectPool<std::string, Shader> m_shaderPool;
    static ObjectPool<std::string, PixelData> m_pixelsPool;

    // TODO: Find out if this OK. May break if Window gets destroyed.
    static const Window* m_window;

    static std::vector<std::string> m_vertShaderExtensions;
    static std::vector<std::string> m_fragShaderExtensions;
    static std::vector<std::string> m_geomShaderExtensions;
    static std::vector<std::string> m_pixelDataExtensions;

    static std::string loadTextViaExtensions(const std::string& baseName, const std::vector<std::string>& extensions);
    static void loadTextFromFile(const std::string& fileName, std::string& dest);
};
