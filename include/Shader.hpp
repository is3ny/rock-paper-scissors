#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
public:
	GLuint id_;
	GLuint vert_, frag_;

	Shader();

	void Compile(const GLchar *vert_src, const GLchar * frag_src, const GLchar * gmt_src);

    // TODO: Unimplemented
    int LoadFromSource(const GLchar *vert_src, const GLchar *frag_src);
	int LoadVertexFromSource(const GLchar *vert_src);
    int LoadFragmentFromSource(const GLchar *frag_src);
    int Link();
    int LinkComp(int n, const GLchar * vars[], GLenum mode);

    const Shader& Use() const;
    // Utility functions
    void SetUniform(const std::string& name, GLfloat value) const;
    void SetUniform(const std::string& name, GLint value) const;
    void SetUniform(const std::string& name, const glm::vec2& value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;
    void SetUniform(const std::string& name, const glm::vec4& value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;
    void SetUniform(const std::string& name, const glm::mat3& value) const;

	void CheckCompileErrors(GLuint id, std::string type);
};


