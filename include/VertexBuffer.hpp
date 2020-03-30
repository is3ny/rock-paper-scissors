#pragma once

#include <vector>
#include "glad/glad.h"
#include "fmt/format.h"

// Looks very similar to the framebuffer class
// TODO: I don't know how to minimize this boilerplate code.
class VertexBuffer
{
public:

    enum UsageType {
        STATIC_DRAW = GL_STATIC_DRAW,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        STREAM_DRAW = GL_STREAM_DRAW
    };

    VertexBuffer()
    {
        glGenBuffers(1, &m_vbo);
    }

    template <typename T>
    VertexBuffer(const std::vector<T>& data, UsageType type)
            : VertexBuffer()
    {
        BufferData(data, type);
    }

    ~VertexBuffer()
    {
        fmt::print("Died!\n");
        glDeleteBuffers(1, &m_vbo);
    }

    GLuint Id()
    {
        return m_vbo;
    }

    void Bind()
    {
        BindCustom(m_vbo);
    }

    template <typename T>
    void BufferData(const std::vector<T>& data, UsageType type)
    {
        auto oldVBO = Current();

        Bind();
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), type);

        BindCustom(oldVBO);
    }

    static void BindCustom(GLuint vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        m_current = vbo;
    }
    
    static void BindDefault()
    {
        BindCustom(0);
    }

    static GLuint Current()
    {
        return m_current;
    }

private:
    GLuint m_vbo;

    static GLuint m_current;
};