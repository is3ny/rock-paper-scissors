#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord_;

out vec2 texCoord;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(pos, 0, 1);
    texCoord = texCoord_;
}