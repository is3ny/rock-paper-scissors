#pragma once

#include "glad/glad.h"
#include "fmt/format.h"

#include "Texture.hpp"

class Framebuffer
{
public:
    enum AttachmentType {
        COLOR = GL_COLOR_ATTACHMENT0,    // Access i-th by addith i to this const
        DEPTH = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };
    
    Framebuffer()
    {
        glGenFramebuffers(1, &m_fbo);
    }

    explicit Framebuffer(int fbo)
            : m_fbo(fbo), foreign(true)
    {
    }

    // Is this good practice? What about the rule of 5?
    Framebuffer(Framebuffer&& other)
            : m_fbo(other.m_fbo), foreign(other.foreign)
    {
    }

    ~Framebuffer()
    {
        if (!foreign)
            glDeleteFramebuffers(1, &m_fbo);
    }

    GLint Id()
    {
        return m_fbo;
    }

    void Bind() const
    {
        BindCustom(m_fbo);
    }

    void AttachTexture(AttachmentType attachment, const Texture& texture)
    {
        // Remember the framebuffer that was previously attached to return it back
        GLint oldFBO = Current();

        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.ID(), 0);

        BindCustom(oldFBO);
    }

    bool IsComplete() const
    {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    static void BindCustom(GLint fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        m_current = fbo;
    }

    static void BindDefault()
    {
        BindCustom(0);
    }

    static GLint Current()
    {
        return m_current;
    }

private:
    GLuint m_fbo;
    bool foreign = false;

    // The expected way to acquire the name of the currently bound fbo is to
    // call glGetIntegerv(GL_FRAMEBUFFER_BINDING, ...), but it always returns zero
    // This variable is used to track currently bound fbo
    static GLint m_current;
};