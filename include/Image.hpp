#pragma once

#include <string>

#include "glm/glm.h"

#include "Shader.hpp"
#include "Texture.hpp"

class Image
{
public:
    Image() = default;
    Image(glm::vec2 pos, glm::vec2 size, const std::string& filepath);
    void draw();

private:
    glm::vec2 m_pos;
    glm::vec2 m_size;
    Texture m_texture;
};