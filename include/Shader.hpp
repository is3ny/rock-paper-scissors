#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "fmt/format.h"

class Shader {
public:
	GLuint id_;
	GLuint vert_, frag_;

	Shader();
    // Destructor should not free the shader program
    ~Shader() = default;

    void Compile(const GLchar *vert_src, const GLchar * frag_src, const GLchar * gmt_src);

    // TODO: Unimplemented
    int LoadFromSource(const GLchar *vert_src, const GLchar *frag_src);
	int LoadVertexFromSource(const GLchar *vert_src);
    int LoadFragmentFromSource(const GLchar *frag_src);
    int Link();
    int LinkComp(int n, const GLchar * vars[], GLenum mode);

    const Shader& Use() const;

    // Utility functions
    template<typename T>
    void SetUniform(const std::string& name, const T& value) const
    {
        Use();
        GLint location = glGetUniformLocation(this->id_, name.data());
        if (location == -1) {
            //fmt::print(stderr, "warning: Trying to assign a value to a uniform that doesn't exist: " + name + "\n");
            return;
        }
    
        setUniform(location, value); 
    }

	void CheckCompileErrors(GLuint id, std::string type);

private:

    void setUniform(GLint loc, GLfloat value) const;
    void setUniform(GLint loc, GLint value) const;
    void setUniform(GLint loc, GLuint value) const;
    void setUniform(GLint loc, const glm::vec2& value) const;
    void setUniform(GLint loc, const glm::vec3& value) const;
    void setUniform(GLint loc, const glm::vec4& value) const;
    void setUniform(GLint loc, const glm::mat4& value) const;
    void setUniform(GLint loc, const glm::mat3& value) const;
};