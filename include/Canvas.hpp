#pragma once

#include "glm/glm.hpp"
#include "Texture.hpp"
#include "Framebuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"

class Canvas
{
public:
    Canvas() = default;
    Canvas(glm::vec2 size);

    // DEPRECATED
    void SetPixel(glm::vec2 pos, glm::vec3 color);

    void SetLine(glm::vec2 start, glm::vec2 end, glm::vec3 color);
    const Texture& GetTexture();

private:
    glm::vec2 m_size;
    
    Framebuffer fbo;
    VertexBuffer vbo;
    VertexArray vao;

    int m_texSelected = 0;
    Texture m_tex1, m_tex2;
};