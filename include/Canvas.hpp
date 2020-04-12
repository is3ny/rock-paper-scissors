#pragma once

#include <vector>
#include <random>
#include <ctime>

#include "glm/glm.hpp"
#include "Texture.hpp"
#include "Framebuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "BufferTexture.hpp"

class Canvas
{
public:
    Canvas() = default;
    Canvas(glm::uvec2 size);

    void DrawLine(glm::vec2 start, glm::vec2 end, glm::vec2 cellInfo, glm::vec2 inputSize = {0, 0});
    void Resize(glm::uvec2 newSize);

    void SetPalette(const std::vector<glm::vec3>& newPalette);
    void Step();

    void GenerateTexture(Texture& out);  // TODO: Maybe this function ideally should be const
    const Texture& GetTexture() const { return m_texBuf[0]; }

    glm::vec2 Size() const { return m_size; }

private:
    glm::uvec2 m_size;

    // General quad spaning entire screen with appropriate texture coords
    // TODO: Refactor them out.
    VertexBuffer m_vbo; 
    VertexArray m_vao;

    // 0 -- ready texture, 1 -- the render target
    std::vector<Texture> m_texBuf{2};
    TextureProperties m_texConf;

    Shader m_drawLineSH;
    Shader m_resizeCanvasSH;
    Shader m_outCanvasSH;
    Shader m_stepSH;

    GLint m_lastPaletteIndex = 0;
    BufferTexture m_paletteBT;

    GLuint m_dsSize = 0;
    BufferTexture m_dsBT;

    BufferTexture m_feedRuleBT;

    std::mt19937 m_rd;
    Texture m_randomMap;
    

    // HACK: some functions need a non-const texture
    Texture& getTexture();

    // HACK(Temporary): Hardcoded maximum value of cell's HPs
    static GLint m_maxLives;
};
