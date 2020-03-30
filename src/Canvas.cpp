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

    //GLuint vbo;
    //glGenBuffers(1, &vbo);
/*  glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
*/
    vbo.BufferData(quad, VertexBuffer::STATIC_DRAW);

    //GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.Id());
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    fbo.Bind();
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    

    fbo.AttachTexture(Framebuffer::COLOR, m_tex2);

    glClearColor(0, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    Framebuffer::BindDefault();
}

void Canvas::SetPixel(glm::vec2 pos, glm::vec3 color)
{
    pos += glm::vec2(0.5, 0.5);
    glm::vec2 ndcPos(pos.x / m_size.x, 1 - pos.y / m_size.y);
    //fmt::print("{}, {}\n", ndcPos.x, ndcPos.y);


    glBindVertexArray(vao);
    auto shader = ResourceManager::GetShader("draw_pixel");
    shader.Use();
    shader.SetUniform("pixelPos", ndcPos);
    shader.SetUniform("desiredColor", color);

    fbo.Bind();
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GetTexture().Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    fbo.AttachTexture(Framebuffer::COLOR, GetTexture());
    m_texSelected ^= 1;
    Framebuffer::BindDefault();
}

void Canvas::SetLine(glm::vec2 start, glm::vec2 end, glm::vec3 color)
{
    // OpenGL positions pixels according to their center
    // TODO: Replace with projection matrix
    // We are working in NDC not in texture coordinates here...
    // That's why we also don't need to 'shift-by-0.5' the coords.
    start = glm::vec2(2) * (start / m_size) - glm::vec2(1);
    end = glm::vec2(2) * (end / m_size) - glm::vec2(1);

    start.y = - start.y;
    end.y =- end.y;

    //fmt::print("From ({}, {}) to ({}, {})\n", start.x, start.y, end.x, end.y);

    std::vector<GLfloat> line = {
        start.x, start.y, 
        end.x, end.y
    };
    //fmt::print("{}: {} {} {} {}\n", sizeof(line), line[0], line[1], line[2], line[3]);

    // Use VBO to store the coordinates of the line and the OpenGL facility to
    // draw really fast
    /*
    VertexBuffer vbo(line, GL_STREAM_DRAW);
    VertexArray vao;
    vao.SetAttribute(0, VAO::VEC2, vbo, 2, 0);

    auto shader = ...
    shader.Use();

    fbo.Bind();
    fbo.AttachTexture(Framebuffer::COLOR_ATTACHMENT, GetTexture());
    glDrawArrays(GL_LINES, 0, 2);

    fbo.Unbind();
    */
    VertexBuffer lineVBO(line, VertexBuffer::STATIC_DRAW);
    //VertexBuffer lineVBO;
    //lineVBO.BufferData(line, VertexBuffer::STATIC_DRAW);
    fmt::print("VBO = {}\n", lineVBO.Id());
    /*
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    */

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    lineVBO.Bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    auto shader = ResourceManager::GetShader("draw_line");
    shader.Use();
    shader.SetUniform("desiredColor", color); 

    // Make framebuffer output to the currently available texture, and then
    // switch back
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //GetTexture().Bind();
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetTexture().id_, 0);
    fbo.AttachTexture(Framebuffer::COLOR, GetTexture());
    fbo.Bind();

    glDrawArrays(GL_LINES, 0, 2);

    m_texSelected ^= 1;
    fbo.AttachTexture(Framebuffer::COLOR, GetTexture());
    m_texSelected ^= 1;

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    VertexBuffer::BindDefault();
    glBindVertexArray(0);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Framebuffer::BindDefault();

    //glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}


const Texture& Canvas::GetTexture()
{
    if (m_texSelected == 0)
        return m_tex1;
    return m_tex2;
}