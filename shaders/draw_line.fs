#version 330 core

out vec4 color;

uniform vec3 desiredColor;

void main() {
    color = vec4(desiredColor, 1);
}