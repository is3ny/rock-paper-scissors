#pragma once

#include "glad/glad.h"

#include "VertexBuffer.hpp"

class BufferTexture
{
public:
    BufferTexture()
    {
        glGenTextures(1, &m_id);
    }

    template <typename T>
    BufferTexture(const std::vector<T>& data, GLenum format)
            : BufferTexture()
    {
        Generate(data, format);
    }

    ~BufferTexture()
    {
        glDeleteTextures(1, &m_id);
    }

    GLuint ID()
    {
        return m_id;
    }

    void Bind(GLuint unit)
    {
        BindCustom(m_id, unit);
    }

    // Will bind to the currently active texture unit
    void Bind()
    {
        BindCustom(m_id);
    }

    template <typename T>
    void Generate(const std::vector<T>& data, GLenum format)
    {
        m_vbo.BufferData(data, VertexBuffer::DYNAMIC_DRAW);
        
        auto oldBT = Current();

        Bind();
        glTexBuffer(GL_TEXTURE_BUFFER, format, m_vbo.Id());

        BindCustom(oldBT);
    }

    // Binds to whatever texture unit is now active
    static void BindCustom(GLuint id)
    {
        glBindTexture(GL_TEXTURE_BUFFER, id);
        m_current = id;
    }
    
    static void BindCustom(GLuint id, GLuint unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_BUFFER, id);
        m_current = id;
    }

    static GLuint Current()
    {
        return m_current;
    }

private:
    GLuint m_id = -1;

    VertexBuffer m_vbo;

    static GLuint m_current;
};