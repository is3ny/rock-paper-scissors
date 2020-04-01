#version 330 core

in vec2 texPos;
out vec4 outColor;

uniform sampler2D automataTex;
uniform samplerBuffer palette;
uniform int paletteSize;

void main()
{
    // The first coordinate encodes the id of the color
    // The second -- HP of the current cell
    vec4 cellInfo = texture(automataTex, texPos);

    // Only round() allows to store ~8M integers in [0, 1] range
    // The floor() and ceil() -- not even a 100.
    // Using zero indexing.
    int id = round(cellInfo.x * paletteSize);

    outColor = texelFetch(palette, id);
}