#include "Framebuffer.hpp"

GLint Framebuffer::m_current = 0;

namespace utils
{

void SetGlobalViewport(const glm::ivec4& vp)
{
    fmt::print("glViewport({}, {}, {}, {})\n", vp.x, vp.y, vp.z, vp.w);
    glViewport(vp.x, vp.y, vp.z, vp.w);
}

} // namespace utils