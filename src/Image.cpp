#include <stdexcept>
#include "stb_image.h"
#include "ImageRenderer.hpp"

#include "Image.hpp"

Shader Image::m_imageShader;

const char* g_imageVert =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec4 vertex;\n"
"\n"
"out vec2 TexCoords;\n"
"\n"
"uniform mat4 projection, model;\n"
"\n"
"void main() {\n"
"    TexCoords = vertex.zw;\n"
"    gl_Position = projection * model * vec4(vertex.xy, 0, 1);\n"
"}\n";

const char* g_imageFrag =
"#version 330 core\n"
"\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"\n"
"uniform sampler2D image;\n"
"uniform vec3 SpriteColor;\n"
"\n"
"void main() {\n"
//"    color =  vec4(SpriteColor, 1.0) * texture(image, TexCoords);\n"
"    color = vec4(SpriteColor, 1.0) * texture(image, TexCoords);\n"
"}\n";

Image::Image()
{
    if (m_imageShader.id_ == -1)
        m_imageShader.Compile(g_imageVert, g_imageFrag, nullptr);
}

Image::Image(glm::vec2 pos, glm::vec2 size, const std::string& filepath)
        : m_pos(pos), m_size(size)
{
    Image();

    unsigned char* data = stbi_load(filepath.data(), 
                                    reinterpret_cast<int*>(&m_texture.width_), 
                                    reinterpret_cast<int*>(&m_texture.height_), nullptr, 0);
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