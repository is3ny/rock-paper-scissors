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

    void SetLine(glm::vec2 start, glm::vec2 end, glm::vec3 color, glm::vec2 inputSize = {0, 0});
    void Resize(glm::vec2 newSize);

    const Texture& GetTexture() const;
    glm::vec2 Size() const { return m_size; }

private:
    glm::vec2 m_size;
    
    Framebuffer m_fbo;
    VertexBuffer m_vbo;
    VertexArray m_vao;

    int m_texSelected = 0;
    Texture m_tex1, m_tex2;

    // HACK: some functions need a non-const texture
    Texture& getTexture();
};