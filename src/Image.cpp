#include <stdexcept>
#include "stb_image.h"
#include "ImageRenderer.hpp"
#include "ResourceManager.hpp"

#include "Image.hpp"

Image::Image()
{
}

Image::Image(glm::vec2 pos, glm::vec2 size, const Texture& texture)
        : m_pos(pos), m_size(size), m_texture(texture)
{
}

Image::Image(glm::vec2 pos, glm::vec2 size, const std::string& filepath)
        : m_pos(pos), m_size(size)
{
    int ch;  // Required to be passed, otherwise segfault (if vertical flip enabled)
    glm::ivec2 imageSize;
    unsigned char* data = stbi_load(filepath.data(), 
                                    reinterpret_cast<int*>(&imageSize.x), 
                                    reinterpret_cast<int*>(&imageSize.y), 
                                    &ch, 
                                    0);
    if (!data) 
        throw std::logic_error("No image data is found at " + filepath);

    // TODO: Infer the right parameters from the ch variable
    TextureProperties texConf;

    // TODO: Support for multiple channels in Texture
    // TODO: Single Responsibility Principle violation
    m_texture.Generate(imageSize, data, texConf);

    stbi_image_free(data);
}

void Image::Draw()
{
    // The direct call to ResourceManager here is needed, because then the projection
    // matrix can be updated. TODO: Find a better way.
    ImageRenderer::DrawImage(ResourceManager::GetShader("image"), m_texture, m_pos, m_size);
}