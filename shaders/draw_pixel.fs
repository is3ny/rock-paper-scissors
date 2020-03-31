#version 330 core

in vec2 texCoord;

uniform vec2 pixelPos;
uniform vec3 desiredColor;
uniform sampler2D tex;

out vec4 color;

void main() {
    //vec2 prox = abs(texCoord - pixelPos);
    //vec2 prox = vec2(abs(texCoord.x - pixelPos.x), abs(texCoord.y - pixelPos.y));
    float dist = sqrt(pow(texCoord.x - pixelPos.x, 2) + pow(texCoord.y - pixelPos.y, 2));
    float eps = 1e-6;
    //if (prox.x < eps && prox.y < eps)
    if (dist < eps)
        color = vec4(desiredColor, 1);
    else
        color = texture(tex, vec2(texCoord.x, texCoord.y));
}
