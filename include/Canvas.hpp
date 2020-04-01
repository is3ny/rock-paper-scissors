#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "Texture.hpp"
#include "Framebuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"

class Canvas
{
public:
    Canvas() = default;
    Canvas(glm::uvec2 size);

    void SetLine(glm::vec2 start, glm::vec2 end, glm::vec3 color, glm::vec2 inputSize = {0, 0});
    void Resize(glm::uvec2 newSize);

    void GenerateTexture(Texture& out) const;
    const Texture& GetTexture() const { return m_texBuf[0]; }
    glm::vec2 Size() const { return m_size; }

private:
    glm::uvec2 m_size;
    
    VertexBuffer m_vbo;
    VertexArray m_vao;

    // 0 -- ready texture, 1 -- the render target
    std::vector<Texture> m_texBuf{2};
    TextureProperties m_texConf;

    // HACK: some functions need a non-const texture
    Texture& getTexture();
};