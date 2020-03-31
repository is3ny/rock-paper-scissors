#version 330 core

layout (location = 0) in vec4 aPos;

out vec2 texCoord;

void main() {
    gl_Position = vec4(aPos.xy, 0, 1);
    texCoord = aPos.zw;
}