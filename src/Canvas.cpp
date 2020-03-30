#include "glad/glad.h"
#include "fmt/format.h"

#include "ResourceManager.hpp"

#include "Canvas.hpp"


Canvas::Canvas(glm::vec2 size)
{
    m_size = size;

    // Generate a texture filled with selected color
    // fully on GPU.

    //GLuint tex;
    glGenTextures(1, &tex1);
    glGenTextures(1, &tex2);
    m_tex1.id_ = tex1;
    m_tex2.id_ = tex2;
    glBindTexture(GL_TEXTURE_2D, tex2);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, tex1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.x, m_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    fbo.AttachTexture(Framebuffer::COLOR, m_tex1);

    if (!fbo.IsComplete())
        fmt::print(stderr, "The framebuffer is incomplete!\n");

    std::vector<GLfloat> quad = {
     //  X     Y     S     T
         1.0,  1.0,  1.0,  1.0,
        -1.0, -1.0,  0.0,  0.0,
        -1.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         1.0, -1.0,  1.0,  0.0,
        -1.0, -1.0,  0.0,  0.0
    };

    vbo.BufferData(quad, VertexBuffer::STATIC_DRAW);
    vao.SetAttribute(0, VertexArray::VEC4, vbo);

    fbo.Bind();
    vao.Bind();
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    

    fbo.AttachTexture(Framebuffer::COLOR, m_tex2);

    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //glBindVertexArray(0);
    VertexArray::BindDefault();
    Framebuffer::BindDefault();
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

    //glBindVertexArray(0);
    VertexArray::BindDefault();

    fbo.AttachTexture(Framebuffer::COLOR, GetTexture());
    m_texSelected ^= 1;
    Framebuffer::BindDefault();
}
*/

void Canvas::SetLine(glm::vec2 start, glm::vec2 end, glm::vec3 color)
{
    // We are working in NDC not in texture coordinates here...
    // We'll convert from the UI coords to NDC: y-inversions welcome!
    // That's why we also don't need to 'shift-by-0.5' the coords.
    glm::mat4 proj(1.0f);
    proj = glm::translate(proj, {-1, 1, 0});
    proj = glm::scale(proj, {2 / m_size.x, -2 / m_size.y, 0});

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
    fbo.AttachTexture(Framebuffer::COLOR, GetTexture());
    fbo.Bind();
    vao.Bind();

    glDrawArrays(GL_LINES, 0, 2);

    m_texSelected ^= 1;
    fbo.AttachTexture(Framebuffer::COLOR, GetTexture());
    m_texSelected ^= 1;

    VertexBuffer::BindDefault();
    VertexArray::BindDefault();
    Framebuffer::BindDefault();
}


const Texture& Canvas::GetTexture()
{
    if (m_texSelected == 0)
        return m_tex1;
    return m_tex2;
}