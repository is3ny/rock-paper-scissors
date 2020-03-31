#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 texCoord;

void main() {
	gl_Position = vec4(aPos, 0, 1);
	texCoord = 0.5 * aPos + 0.5;
}
