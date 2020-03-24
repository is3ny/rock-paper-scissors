#include "Image.hpp"

const char* g_imageVert =
"";

const char* g_imageFrag =
"";

Image::Image()
{
    if (m_imageShader.id_ == -1)
        m_imageShader.Compile(g_imageVert, g_imageFrag, nullptr);
}

Image::Image(glm::vec2 pos, glm::vec2 size, const std::string& filepath)
        : m_pos(pos), m_size(size)
{
    Image();

    unsigned char* data = stbi_load(filepath.data(), &m_texture.width_, &m_texture.height_, nullptr, 0);
    if (!data) 
        throw std::logic_error("No image data is found at " + filepath);

    // TODO: Support for multiple channels in Texture
    m_texture.Generate(data);

    stbi_image_free(data);

}