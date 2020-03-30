#pragma once

#include <map>
#include "glad/glad.h"

#include "VertexBuffer.hpp"

class VertexArray
{
public:
    enum AttribType {
        FLOAT,
        VEC2,
        VEC3,
        VEC4
    };

    struct AttribTypeDetails
    {
        GLint compCount;    // of components
        GLenum type;
        GLint byteSize;
    };

    VertexArray()
    {
        glGenVertexArrays(1, &m_vao);
    }
    
    ~VertexArray()
    {
        glDeleteVertexArrays(1, &m_vao);
    }

    GLuint Id()
    {
        return m_vao;
    }

    void Bind()
    {
        BindCustom(m_vao);
    }

    void SetAttribute(GLuint index, AttribType type, const VertexBuffer& vbo, int stride = 0, int first = 0)
    {
        auto oldVAO = Current();

        auto params = m_attribParams[type];

        Bind();
        vbo.Bind();
        glVertexAttribPointer(index, 
                              params.compCount, 
                              params.type, 
                              GL_FALSE, 
                              stride * params.byteSize, 
                              (void*)(intptr_t)(first * params.byteSize));
        glEnableVertexAttribArray(index);
        
        BindCustom(oldVAO);
    }

    static void BindCustom(GLuint vao)
    {
        glBindVertexArray(vao);
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
    GLuint m_vao;

    static GLuint m_current;
    static std::map<AttribType, AttribTypeDetails> m_attribParams; 
};