#version 330 core

layout (location = 0) in vec4 vert;

out vec2 texCoord;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(vert.xy, 0, 1);
    texCoord = vert.zw;
}