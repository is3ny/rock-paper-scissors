#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

class Image
{
public:
    Image();
    // TODO: Maybe Texture should be const
    Image(glm::vec2 pos, glm::vec2 size, const Texture& texture);
    Image(glm::vec2 pos, glm::vec2 size, const std::string& filepath);

    glm::vec2 Pos()
    {
        return m_pos;
    }

    void SetPos(glm::vec2 pos)
    {
        m_pos = pos;
    }

    glm::vec2 Size()
    {
        return m_size;
    }

    void SetSize(glm::vec2 size)
    {
        m_size = size;
    }

    // TODO: getters supposed to return const values. I think I need to
    // rework how canvas outputs the 
    Texture& GetTexture()
    {
        return m_texture;
    }

    void SetTexture(const Texture& texture)
    {
        m_texture = texture;
    }

    void Draw();

private:
    glm::vec2 m_pos;
    glm::vec2 m_size;
    Texture m_texture;
};

