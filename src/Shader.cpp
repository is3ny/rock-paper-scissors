#include "fmt/format.h"

#include "Shader.hpp"

Shader::Shader() {
	this->id_ = -1;
}

Shader::~Shader()
{
    if (this->id_ != -1)
        glDeleteProgram(this->id_);
}

void Shader::Compile(const GLchar * vert_src, const GLchar * frag_src, const GLchar * gmt_src) {
    GLuint vert, frag, gmt;
    // Vertex shader
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_src, NULL);
    glCompileShader(vert);
    CheckCompileErrors(vert, "VERTEX shader");
    // Fragment shader
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_src, NULL);
    glCompileShader(frag);
    CheckCompileErrors(frag, "FRAGMENT shader");
    // Geometry shader, if needed
    if (gmt_src) {
        gmt = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gmt, 1, &gmt_src, NULL);
        glCompileShader(gmt);
        CheckCompileErrors(gmt, "GEOMETRY shader");
    }
    // Shader program
    this->id_ = glCreateProgram();
    glAttachShader(this->id_, vert);
    glAttachShader(this->id_, frag);
    if (gmt_src)
        glAttachShader(this->id_, gmt);
    glLinkProgram(this->id_);
    CheckCompileErrors(this->id_, "PROGRAM");
    // Delete shaders as they are already linked to our program
    glDeleteShader(vert);
    glDeleteShader(frag);
    if (gmt_src) 
        glDeleteShader(gmt);

}

/*
int Shader::LinkComp(int n, const GLchar * vars[], GLenum mode) {
    id = glCreateProgram();
    glAttachShader(id, vert);

    std::cout << "Transform Feedback vars:\n";
    for (int i = 0; i < n; i++)
        std::cout << vars[i] << std::endl;
    glTransformFeedbackVaryings(id, n, vars, mode);

    glLinkProgram(id);
    glLinkProgram(id);
    if (!programStatus(id)) {
        return -1;
    }

    //Clean up
    glDeleteShader(vert);
    return 0;
}
*/

const Shader& Shader::Use() const {
    glUseProgram(this->id_);
	return *this;
}

void Shader::SetUniform(const std::string& name, GLfloat value) const
{
    glUniform1f(glGetUniformLocation(this->id_, name.data()), value);
}

void Shader::SetUniform(const std::string& name, GLint value) const
{
    glUniform1i(glGetUniformLocation(this->id_, name.data()), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value) const
{
    glUniform2f(glGetUniformLocation(this->id_, name.data()), value.x, value.y);
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(this->id_, name.data()), value.x, value.y, value.z);
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value) const
{
    glUniform4f(glGetUniformLocation(this->id_, name.data()), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->id_, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->id_, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::CheckCompileErrors(GLuint object, std::string type) {
    GLint success;
    GLchar log[1024];
    if (type == "PROGRAM") {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, NULL, log);
            fmt::print("ERROR::SHADER: Link-time error. Type: {}\n", type);
            fmt::print("{}\n -- ------------------------------------------- --\n", log);
        }
    } else {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, log);
            fmt::print("ERROR::SHADER: Compile-time error. Type: {}\n", type);
            fmt::print("{}\n -- ------------------------------------------- --\n", log);
        }
    }
}
// END ----- Shader
