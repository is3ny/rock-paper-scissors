#pragma once

#include "glm/glm.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

// A static class that provides an interface for rendering
// textures at specified positions.
class ImageRenderer
{
public:
    /// Populate buffer with verticies for a 1x1 square with top-left corner located at (0,0)
    static void Init();
    static void CleanUp();

    static void DrawImage(const Shader&,
                          const Texture&, 
                          glm::vec2 pos = {0, 0}, 
                          glm::vec2 size = {32, 32}, 
                          float angle = 0.0f, 
                          glm::vec3 color = {0, 0, 0});

private:
    static GLuint m_quadVBO;
    static GLuint m_quadVAO;
};

GLuint ImageRenderer::m_quadVBO = 0;
GLuint ImageRenderer::m_quadVAO = 0;