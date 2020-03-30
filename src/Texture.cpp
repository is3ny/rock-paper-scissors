#include <stdexcept>

#include "Texture.hpp"

Texture::Texture()
        : m_id(-1)
{
}

void Texture::Generate(glm::uvec2 size, const void* data, TextureProperties props) 
{
    m_size = size;
    m_props = props;

    if (m_id == -1)
        glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 
                 0,                         // Mipmap level: 0 = base image 
                 m_props.internalFormat, 
                 m_size.x, 
                 m_size.y, 
                 0,                         // Should be always zero
                 m_props.imageFormat, 
                 GL_UNSIGNED_BYTE, 
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_props.sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_props.tWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_props.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_props.magFilter);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const {
    if (m_id == -1)
        throw std::runtime_error("Trying to bind uninitialized texture");

    BindCustom(m_id);
}


void Texture::BindCustom(GLuint name)
{
    glBindTexture(GL_TEXTURE_2D, name);
}