#include "ImageRenderer.hpp"


void ImageRenderer::Init()
{
    // Vertex data, the first vertex is at the top-left corner
    GLfloat data[] {
    //  X     Y     S     T
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    // Initialize and update quad buffer
    glGenBuffers(1, &m_quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    // Initialize VAO and attributes
    glGenVertexArrays(1, &m_quadVAO);
    glBindVertexArray(m_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ImageRenderer::CleanUp()
{
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);
}

void ImageRenderer::DrawImage(const Shader& shader,
                              const Texture& texture, 
                              glm::vec2 pos,
                              glm::vec2 size,
                              float angle,
                              glm::vec3 color)
{
    // Prepare transformations
    glm::mat4 model(1.0f);
    // Move to the desired position
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    // Rotate around the origin (since (0,0) is at top-left of the quad
    // We move its center to (0,0), rotate and revert
    model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));
    // Scale 1x1 quad to the appropriate size
    model = glm::scale(model, glm::vec3(size, 1.0f));
    //Initialize shader
    shader.Use();
    shader.SetMatrix4("model", model);
    shader.SetVector3f("SpriteColor", color);
    // Bind the proper texture
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    // Draw sprite
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}