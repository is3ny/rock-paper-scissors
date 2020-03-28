#include <fstream>
#include <sstream>

#include "fmt/format.h"

#include "ResourceManager.hpp"

ObjectPool<std::string, Shader> ResourceManager::m_shaderPool;
ObjectPool<std::string, PixelData> ResourceManager::m_pixelsPool;

const Window* ResourceManager::m_window;

std::vector<std::string> ResourceManager::m_vertShaderExtensions = { ".vs", ".vert" };
std::vector<std::string> ResourceManager::m_fragShaderExtensions = { ".fs", ".frag" };
std::vector<std::string> ResourceManager::m_geomShaderExtensions = { ".gs", ".geom" };
std::vector<std::string> ResourceManager::m_pixelDataExtensions = { ".png", ".jpg", ".jpeg", ".JPG", ".JPEG" };

void ResourceManager::Init(const Window* window)
{
    m_window = window;
}

void ResourceManager::LoadShader(const std::string& name)
{
    std::string vertSource = loadTextViaExtensions(name, m_vertShaderExtensions);
    std::string fragSource = loadTextViaExtensions(name, m_fragShaderExtensions);
    std::string geomSource = loadTextViaExtensions(name, m_geomShaderExtensions);

    // throw if either vert or frag source is empty, since they are necessary
    if (vertSource.empty())
        throw std::runtime_error("Shader " + name + " has no vertex shader source");
    if (fragSource.empty())
        throw std::runtime_error("Shader " + name + " has no fragment shader source");

    // It will be a rarity to see geometry shader being loaded
    const char* geomSourceData = nullptr;
    if (!geomSource.empty()) {
        fmt::print(stderr, "info: Shader " + name + " has geometry shader loaded\n");
        geomSourceData = geomSource.data();
    }

    Shader shader;
    shader.Compile(vertSource.data(), fragSource.data(), geomSourceData);

    m_shaderPool[name] = shader;
}


std::string ResourceManager::loadTextViaExtensions(const std::string& baseName, 
                                                   const std::vector<std::string>& extensions)
{
    std::string res = "";
    for (auto& ext : extensions) {
        loadTextFromFile(baseName + ext, res);
        if (!res.empty())
            return res;
    }

    return res;
}

void ResourceManager::loadTextFromFile(const std::string& fileName, std::string& dest)
{
    std::ifstream file(fileName);
    if (!file.is_open())
        return;

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    dest = ss.str();
}

Shader& ResourceManager::GetShader(const std::string& name)
{
 
    if (m_shaderPool.find(name) == m_shaderPool.end())
        throw std::domain_error("Request for inexistent object");
    return m_shaderPool[name];
}

PixelData& ResourceManager::GetPixels(const std::string& name)
{
    if (m_pixelsPool.find(name) == m_pixelsPool.end())
        throw std::domain_error("Request for inexistent object");
    return m_pixelsPool[name];
}

void ResourceManager::Clear()
{
    for (auto& shader : m_shaderPool)
        glDeleteShader(shader.second.id_);
    //m_pixelsPool.Clear();
}