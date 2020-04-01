#version 330 core

layout (location = 0) in vec4 vPos;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(vPos.xy, 0, 1);
    texCoord = vPos.zw;
}