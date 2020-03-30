#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

struct TextureProperties
{
    GLint internalFormat = GL_RGB;
    GLint imageFormat = GL_RGB;
    GLint magFilter = GL_NEAREST;
    GLint minFilter = GL_NEAREST;
    GLint sWrap = GL_REPEAT;
    GLint tWrap = GL_REPEAT;
};

class Texture {
public:
    // Constructor: sets default texture modes
    Texture();
    // Generates texture from image data with the above parameters and generates an id
    //void Generate(unsigned char * data, GLuint width = 0, GLuint height = 0);
    // The only way to acquire an ID for the texture
    void Generate(glm::uvec2 size, const void* data, TextureProperties props);
    // Binds the texture as the current GL_TEXTURE_2D texture object
    void Bind() const;

    GLuint ID() const                    { return m_id; }
    glm::uvec2 Size() const              { return m_size; }
    TextureProperties Properties() const { return m_props; }

    static void BindCustom(GLuint name);

private:
    GLuint m_id;
    glm::uvec2 m_size;
    TextureProperties m_props;
};
