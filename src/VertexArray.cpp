#include "VertexArray.hpp"

GLuint VertexArray::m_current = 0;

std::map<VertexArray::AttribType, VertexArray::AttribTypeDetails> VertexArray::m_attribParams {
    {VertexArray::FLOAT, { 1, GL_FLOAT, sizeof(float) }},
    {VertexArray::VEC2,  { 2, GL_FLOAT, sizeof(float) }},
    {VertexArray::VEC3,  { 3, GL_FLOAT, sizeof(float) }},
    {VertexArray::VEC4,  { 4, GL_FLOAT, sizeof(float) }},
};