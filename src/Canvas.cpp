#include "glad/glad.h"
#include "fmt/format.h"

#include "ResourceManager.hpp"

#include "Canvas.hpp"


Canvas::Canvas(glm::uvec2 size)
{
    m_size = size;

    TextureProperties texConf;
    texConf.magFilter = GL_LINEAR;
    texConf.minFilter = GL_LINEAR;
    texConf.internalFormat = texConf.imageFormat = GL_RGB;

    // By passing nullptr we are telling GPU to just allocate the memory
    m_texBuf[0].Generate(m_size, nullptr, texConf);
    m_texBuf[1].Generate(m_size, nullptr, texConf);

    std::vector<GLfloat> quad = {
     //  X     Y     S     T
         1.0,  1.0,  1.0,  1.0,
        -1.0, -1.0,  0.0,  0.0,
        -1.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         1.0, -1.0,  1.0,  0.0,
        -1.0, -1.0,  0.0,  0.0
    };

    // TODO: Make stream draw
    m_vbo.BufferData(quad, VertexBuffer::STATIC_DRAW);
    m_vao.SetAttribute(0, VertexArray::VEC4, m_vbo);

    Framebuffer fbo;
    fbo.Bind();

    m_vao.Bind();

    // This will basically fill the texture with the clear color
    // But the drawArrays call is still needed
    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[0]);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[1]);
    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    VertexArray::BindDefault();
    fbo.Unbind();
}

void Canvas::SetLine(glm::vec2 start, glm::vec2 end, glm::vec3 color, glm::vec2 inputSize)
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

    auto shader = ResourceManager::GetShader("draw_line");
    shader.Use();
    shader.SetUniform("desiredColor", color);
    shader.SetUniform("proj", proj);

    // Make framebuffer output to the currently available texture, and then
    // switch back
    Framebuffer fbo(m_size);
    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[0]);
    fbo.Bind();
    vao.Bind();

    glDrawArrays(GL_LINES, 0, 2);

    fbo.Unbind();

    VertexBuffer::BindDefault();
    VertexArray::BindDefault();
}

void Canvas::Resize(glm::uvec2 newSize)
{

    // 1. Clear texture and allocate for new dimension
    // 2. Prepare projection matrix
    // 3. Bind FBO, VAO and render
    // 4. Clean up

    // TODO: TextureProperties probably have to be exported to the private
    // class member.
    m_texBuf[1].Generate(newSize, nullptr, TextureProperties());

    Framebuffer fbo(newSize);
    fbo.AttachTexture(Framebuffer::COLOR, m_texBuf[1]);

    if (!fbo.IsComplete())
        fmt::print("Aha!\n");

    glm::mat4 proj(1.0);
    proj = glm::translate(proj, {-1, 1, 0});
    proj = glm::scale(proj, {2.0f * m_size.x / newSize.x, 2.0f * m_size.y / newSize.y, 0});

    // (0, 0) -- (1, -1)
    std::vector<GLfloat> quad = {
     //  X     Y     S     T
         0.0,  0.0,
         1.0,  0.0,
         0.0, -1.0,
         1.0,  0.0,
         0.0, -1.0,
         1.0, -1.0
    };

    std::vector<GLfloat> texCoords = {
        0.0, 1.0,
        1.0, 1.0,
        0.0, 0.0,
        1.0, 1.0,
        0.0, 0.0, 
        1.0, 0.0
    };

    glm::vec2 s, e;
    s = proj * glm::vec4(0, 0, 0, 1);
    e = proj * glm::vec4(1, -1, 0, 1);
    fmt::print("\nTex rect: ({}, {}) -- ({}, {})\n", s.x, s.y, e.x, e.y);

    VertexBuffer quadVBO(quad, VertexBuffer::STREAM_DRAW);
    VertexBuffer texCoordVBO(texCoords, VertexBuffer::STREAM_DRAW);

    VertexArray vao;
    vao.SetAttribute(0, VertexArray::VEC2, quadVBO);
    vao.SetAttribute(1, VertexArray::VEC2, texCoordVBO);

    auto shader = ResourceManager::GetShader("resize_canvas");
    shader.Use();
    shader.SetUniform("proj", proj);
    
    fbo.Bind();
    vao.Bind();
    GetTexture().Bind();

    glClearColor(1, 0, 1, 1); // Border color
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: Replace with Triangle fan
    glDrawArrays(GL_TRIANGLES, 0, 6);

    VertexArray::BindDefault();
    fbo.Unbind();

    //fmt::print("Old = {} {}", m_texBuf[0].ID());
    std::swap(m_texBuf[0], m_texBuf[1]);
    fmt::print("New = {} {}\n", m_texBuf[0].ID(), m_texBuf[1].ID());

    // The texture to be rendered next also has to have new dimensions
    m_texBuf[1].Generate(newSize, nullptr, TextureProperties());

    m_size = newSize;
}

Texture& Canvas::getTexture()
{
    return m_texBuf[0];
}

void Canvas::GenerateTexture(Texture& out) const
{
    /*
    Framebuffer fbo(m_size);
    fbo.AttachTexture(Framebuffer::COLOR, out);

    if (!out.Valid() || out.Size() != m_size)
        out.Generate(m_size, nullptr, texConf
    */
}




/*
void Canvas::SetPixel(glm::vec2 pos, glm::vec3 color)
{
    pos += glm::vec2(0.5, 0.5);
    glm::vec2 ndcPos(pos.x / m_size.x, 1 - pos.y / m_size.y);


    //glBindVertexArray(vao);
    vao.Bind();
    auto shader = ResourceManager::GetShader("draw_pixel");
    shader.Use();
    shader.SetUniform("pixelPos", ndcPos);
    shader.SetUniform("desiredColor", color);

    fbo.Bind();
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GetTexture().Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    VertexArray::BindDefault();

    fbo.AttachTexture(Framebuffer::COLOR, GetTexture());
    m_texSelected ^= 1;
    Framebuffer::BindDefault();
}
*/