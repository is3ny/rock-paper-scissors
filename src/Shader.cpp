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

void Shader::setUniform(GLuint loc, GLfloat value) const
{
    glUniform1f(loc, value);
}

void Shader::setUniform(GLuint loc, GLint value) const
{
    glUniform1i(loc, value);
}

void Shader::setUniform(GLuint loc, const glm::vec2& value) const
{
    glUniform2f(loc, value.x, value.y);
}

void Shader::setUniform(GLuint loc, const glm::vec3& value) const
{
    glUniform3f(loc, value.x, value.y, value.z);
}

void Shader::setUniform(GLuint loc, const glm::vec4& value) const
{
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Shader::setUniform(GLuint loc, const glm::mat4& value) const
{
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(GLuint loc, const glm::mat3& value) const
{
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
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
