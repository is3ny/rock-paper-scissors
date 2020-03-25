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
                          glm::vec3 color = {1, 1, 1});

private:
    static GLuint m_quadVBO;
    static GLuint m_quadVAO;
};
