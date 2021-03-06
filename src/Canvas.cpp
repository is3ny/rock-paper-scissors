#include "glad/glad.h"
#include "fmt/format.h"

#include "Mesh.hpp"
#include "ResourceManager.hpp"

#include "Canvas.hpp"

GLint Canvas::m_maxLives = 10;

Canvas::Canvas(glm::uvec2 size)
{
    m_size = size;
    m_rd = std::mt19937(time(0));

    m_drawLineSH = ResourceManager::GetShader("draw_line");
    m_resizeCanvasSH = ResourceManager::GetShader("resize_canvas");
    m_outCanvasSH = ResourceManager::GetShader("canvas_out");
    m_stepSH = ResourceManager::GetShader("step");

    m_texConf.magFilter = GL_NEAREST;
    m_texConf.minFilter = GL_NEAREST;
    //m_texConf.internalFormat = m_texConf.imageFormat = GL_RGB;
    m_texConf.sWrap = m_texConf.tWrap = GL_CLAMP_TO_EDGE;

    // By passing nullptr we are telling GPU to just allocate the memory
    m_texBuf[0].Generate(m_size, nullptr, m_texConf);
    m_texBuf[1].Generate(m_size, nullptr, m_texConf);

    m_vbo.BufferData(mesh::screenQuadTx, VertexBuffer::STREAM_DRAW);
    m_vao.SetAttribute(0, VertexArray::VEC4, m_vbo);

    Framebuffer fbo;
    fbo.Bind();

    m_vao.Bind();

    ResourceManager::GetShader("image").Use();

    // This will basically fill the texture with the clear color
    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[0]);
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[1]);
    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);


    // The displacements for all 8 neigbors of a cell
    std::vector<GLubyte> ds = {
        255, 128,
        128, 255,
          0, 128,
        128,   0,
        255, 255,
          0, 255,
          0,   0,
        255,   0 
    };
    m_dsBT.Generate(ds, GL_RG8);


    std::vector<GLubyte> feedMap = {
        0, 0, 0, 0,
        255, 0, 255, 0,
        255, 0, 0, 255,
        255, 255, 0, 0
    };
    m_feedRuleBT.Generate(feedMap, GL_R8);

    m_stepSH.Use();
    m_stepSH.SetUniform("dsSize", static_cast<GLint>(ds.size()) / 2);
    m_stepSH.SetUniform("maxLives", m_maxLives);
    m_stepSH.SetUniform("dpixel", glm::vec2(1.0 / m_size.x, 1.0 / m_size.y));


    TextureProperties randMapProps;
    randMapProps.internalFormat = randMapProps.imageFormat = GL_RED;

    glm::uvec2 randSize{2000, 2000};
    fmt::print(stderr, "Generating {}x{} random map...", randSize.x, randSize.y);
    std::uniform_int_distribution<GLubyte> dist(0, 255);
    std::vector<GLubyte> randMap(randSize.x * randSize.y);
    for (int i = 0; i < randMap.size(); ++i) {
        randMap[i] = dist(m_rd);
    }
    fmt::print("\n");

    m_randomMap.Generate(randSize, &randMap[0], randMapProps);
    fmt::print(stderr, "ok\n");
}

void Canvas::DrawLine(glm::vec2 start, glm::vec2 end, glm::vec2 cellInfo, glm::vec2 inputSize)
{
    // We are working in NDC not in texture coordinates here...
    // We'll convert from the UI coords to NDC: y-inversions welcome!
    // That's why we also don't need to 'shift-by-0.5' the coords.
    if (inputSize == glm::vec2(0, 0))
        inputSize = m_size;

    glm::mat4 proj(1.0f);
    proj = glm::ortho(0.0f, inputSize.x, inputSize.y, 0.0f, -1.0f, 1.0f);

    std::vector<GLfloat> line = {
        start.x, start.y, 
        end.x, end.y
    };

    // Use VBO to store the coordinates of the line and the OpenGL facility to
    // draw really fast
    VertexBuffer lineVBO(line, VertexBuffer::STATIC_DRAW);

    VertexArray vao;
    vao.SetAttribute(0, VertexArray::VEC2, lineVBO);

    // This relies on the fact that cellInfo is a float vec
    glm::vec3 cellColor(cellInfo.x / m_lastPaletteIndex, cellInfo.y / m_maxLives, 0);

    m_drawLineSH.Use();
    m_drawLineSH.SetUniform("desiredColor", cellColor);
    m_drawLineSH.SetUniform("proj", proj);

    // Make framebuffer output to the currently available texture, and then
    // switch back
    Framebuffer fbo(m_size);
    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[0]);
    fbo.Bind();
    vao.Bind();

    glDrawArrays(GL_LINES, 0, 2);
}

void Canvas::Resize(glm::uvec2 newSize)
{

    // 1. Clear texture and allocate for new dimension
    // 2. Prepare projection matrix
    // 3. Bind FBO, VAO and render
    // 4. Clean up

    m_texBuf[1].Generate(newSize, nullptr, m_texConf);

    fmt::print("リサーイズ event start!\n");
    Framebuffer fbo(newSize);
    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[1]);

    if (!fbo.IsComplete())
        fmt::print("Aha!\n");
        
    VertexBuffer vbo(mesh::oct4QuadTx, VertexBuffer::STREAM_DRAW);
    VertexArray vao;
    vao.SetAttribute(0, VertexArray::VEC4, vbo);

    glm::mat4 proj(1.0);
    proj = glm::translate(proj, {-1, 1, 0});
    proj = glm::scale(proj, {2.0f * m_size.x / newSize.x, 2.0f * m_size.y / newSize.y, 0});

    m_resizeCanvasSH.Use();
    m_resizeCanvasSH.SetUniform("proj", proj);
    
    fbo.Bind();
    vao.Bind();
    GetTexture().Bind();

    glClearColor(1, 0, 1, 1); // Border color
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Replace with Triangle fan
    glDrawArrays(GL_TRIANGLES, 0, 6);


    std::swap(m_texBuf[0], m_texBuf[1]);
    fmt::print("New = {} {}\n", m_texBuf[0].ID(), m_texBuf[1].ID());

    // The texture to be rendered next also has to have new dimensions
    m_texBuf[1].Generate(newSize, nullptr, m_texConf);

    m_size = newSize;

    // Since resize events are fairly rare, it's more efficient to set
    // pixel size in texture coords for the shaders here.
    m_stepSH.Use();
    m_stepSH.SetUniform("dpixel", glm::vec2(1.0 / m_size.x, 1.0 / m_size.y));

    fmt::print("リサーイズ event end!\n");
}

Texture& Canvas::getTexture()
{
    return m_texBuf[0];
}

void Canvas::GenerateTexture(Texture& out)
{
    if (!out.Valid() || out.Size() != m_size)
        out.Generate(m_size, nullptr, m_texConf);

    Framebuffer fbo(m_size);
    fbo.AttachTexture(Framebuffer::COLOR, out);


    //auto shader = ResourceManager::GetShader("canvas_out");
    m_outCanvasSH.Use();
    m_outCanvasSH.SetUniform("palette", 1);
    
    fbo.Bind();
    m_vao.Bind();    

    m_paletteBT.Bind(1);
    m_texBuf[0].Bind(0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Canvas::SetPalette(const std::vector<glm::vec3>& newPalette)
{
    m_lastPaletteIndex = newPalette.size() - 1;

    std::vector<GLubyte> palette(4 * newPalette.size());
    for (int pi = 0; pi < newPalette.size(); ++pi) {
        for (int off = 0; off < 3; ++off)
            palette[4 * pi + off] = round(255 * newPalette[pi][off]);

        palette[4 * pi + 3] = 255;    // Alpha component
    }

    // Even though we don't need alpha channel, it turns out RGBA8 is the best
    // option here, since it's smaller than all image formats with 3 components
    // and the uintegers are normalized.
    m_paletteBT.Generate(palette, GL_RGBA8);

    m_outCanvasSH.Use();
    m_outCanvasSH.SetUniform("lastPaletteIndex", m_lastPaletteIndex);

    m_stepSH.Use();
    m_stepSH.SetUniform("lastColorIndex", m_lastPaletteIndex);
    fmt::print("lastColorIndex = {}\n", m_lastPaletteIndex);
}

void Canvas::Step()
{
    VertexBuffer vbo(mesh::screenQuadTx, VertexBuffer::STATIC_DRAW);
    VertexArray vao;
    vao.SetAttribute(0, VertexArray::VEC4, vbo);

    Framebuffer fbo(m_size);
    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[1]);

    std::uniform_int_distribution<GLint> dist(0, m_size.x);
    glm::vec2 seed(dist(m_rd), dist(m_rd));

    m_stepSH.Use();
    m_stepSH.SetUniform("seed", seed);
    m_stepSH.SetUniform("ds", 1);
    m_stepSH.SetUniform("feedRule", 2);
    m_stepSH.SetUniform("randomMap", 3);

    fbo.Bind();
    vao.Bind();
    m_texBuf[0].Bind(0);
    m_dsBT.Bind(1);
    m_feedRuleBT.Bind(2);
    m_randomMap.Bind(3);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    std::swap(m_texBuf[0], m_texBuf[1]);
}
