#pragma once

#include "glad/glad.h"
#include "fmt/format.h"

#include "Texture.hpp"

namespace utils
{
    void SetGlobalViewport(const glm::ivec4&);
} // namespace utils

class Framebuffer
{
public:
    enum AttachmentType {
        COLOR = GL_COLOR_ATTACHMENT0,    // Access i-th attachment by adding i to this constant
        DEPTH = GL_DEPTH_ATTACHMENT,
        STENCIL = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
    };
    
    Framebuffer()
    {
        glGenFramebuffers(1, &m_fbo);
    }

    Framebuffer(const glm::ivec4& vp)
            : m_viewport(vp)
    {
        glGenFramebuffers(1, &m_fbo);
    }

    Framebuffer(const glm::ivec2& vp)
    {
        m_viewport = glm::ivec4(0, 0, vp.x, vp.y);
        glGenFramebuffers(1, &m_fbo);
    }

    explicit Framebuffer(int fbo)
            : m_fbo(fbo), m_foreign(true)
    {
    }

    ~Framebuffer()
    {
        if (!m_foreign) {
            Unbind();
            glDeleteFramebuffers(1, &m_fbo);
        }
    }

    GLuint ID() const
    {
        return m_fbo;
    }

    void SetViewport(glm::ivec4 viewport)
    {
        m_viewport = viewport;
    }

    glm::ivec4 GetViewport() const
    {
        return m_viewport;
    }

    void Bind()
    {
        if (m_bound)
            return;
            
        auto cur = Current();

        if (m_fbo == m_current) {
            if (cur.GetViewport() != m_viewport)
                utils::SetGlobalViewport(m_viewport);
            return;
        }

        m_prevFBO = cur.ID();
        m_prevViewport = cur.GetViewport();

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        utils::SetGlobalViewport(m_viewport);
        m_current = m_fbo;
        m_bound = true;
    }

    void Unbind()
    {
        if (!m_bound || m_fbo != m_current)
            return;

        if (glGetError() != GL_NO_ERROR) {
            fmt::print(stderr, "warning: Error before Unbind {}\n", m_prevFBO);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_prevFBO);
        if (glGetError() != GL_NO_ERROR) {
            fmt::print(stderr, "warning: Could not bind framebuffer {}\n", m_prevFBO);
        }
        
        utils::SetGlobalViewport(m_prevViewport);
        m_current = m_prevFBO;
        m_bound = false;
    }

    void AttachTexture(AttachmentType attachment, const Texture& texture)
    {
        Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.ID(), 0);
        Unbind();
    }

    bool IsComplete() const
    {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    static Framebuffer Current()
    {
        Framebuffer cur(m_current);

        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        cur.m_viewport = {vp[0], vp[1], vp[2], vp[3]};

        return cur;
    }

private:
    GLuint m_fbo;
    bool m_foreign = false;
    glm::ivec4 m_viewport;

    GLuint m_prevFBO;
    glm::ivec4 m_prevViewport;
    bool m_bound = false;

    // The expected way to acquire the name of the currently bound fbo is to
    // call glGetIntegerv(GL_FRAMEBUFFER_BINDING, ...), but it always returns zero
    // This variable is used to track currently bound fbo
    static GLint m_current;
};
