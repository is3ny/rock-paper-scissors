#include <stdexcept>
#include "stb_image.h"
#include "ImageRenderer.hpp"
#include "ResourceManager.hpp"

#include "Image.hpp"

Shader Image::m_imageShader;

Image::Image()
{
    if (m_imageShader.id_ == -1)
        m_imageShader = ResourceManager::GetShader("image");
}

Image::Image(glm::vec2 pos, glm::vec2 size, const Texture& texture)
        : m_pos(pos), m_size(size), m_texture(texture)
{
    Image();
}

Image::Image(glm::vec2 pos, glm::vec2 size, const std::string& filepath)
        : m_pos(pos), m_size(size)
{
    Image();

    int ch;  // Required to be passed, otherwise segfault (if vertical flip enabled)
    unsigned char* data = stbi_load(filepath.data(), 
                                    reinterpret_cast<int*>(&m_texture.width_), 
                                    reinterpret_cast<int*>(&m_texture.height_), &ch, 0);
    if (!data) 
        throw std::logic_error("No image data is found at " + filepath);

    // TODO: Support for multiple channels in Texture
    // TODO: Single Responsibility Principle violation
    m_texture.Generate(data);

    stbi_image_free(data);
}

void Image::Draw()
{
    ImageRenderer::DrawImage(m_imageShader, m_texture, m_pos, m_size);
}