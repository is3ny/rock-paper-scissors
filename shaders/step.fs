#version 330 core

in vec2 texCoord;
out vec4 outColor;

// Array of possible shifts
// Format: (dx1, dy1), (dx2, dy2), ...
// Additionally, the displacements need to be mapped from [0, 1] to [-1, 1]
uniform samplerBuffer ds;
uniform int dsSize;
// TODO: set dsSize

// Matrix that tells what types eat other types -- an R8 buffer
// Row x Column = 1 --> Row color eats Column
uniform samplerBuffer feedRule;
uniform int lastColorIndex;
// TODO: Update ^ when palette changes

uniform int maxLives;
// TODO: Initialize in Canvas

uniform vec2 seed;

uniform mat4 toNDC;
uniform mat4 ndcToTex;

uniform sampler2D automata;

// 2D Random
float Random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float Noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = Random(i);
    float b = Random(i + vec2(1.0, 0.0));
    float c = Random(i + vec2(0.0, 1.0));
    float d = Random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

ivec2 GetCellInfo(vec4 texel)
{
    return ivec2(int(round(texel.x * lastColorIndex)), int(round(texel.y * maxLives)));
}

vec4 GetCellTexel(vec2 coord)
{
    vec4 tcoord = ndcToTex * toNDC * vec4(coord, 0, 1);
    return texture(automata, tcoord.xy);
}

void main()
{
    vec4 cellTexel = GetCellTexel(texCoord);
    ivec2 cellInfo = GetCellInfo(cellTexel);

    // Choose random neighbor
    int choice = int(round(Noise(texCoord + seed) * (dsSize - 1)));
    vec2 shift = 2 * texelFetch(ds, choice).xy - vec2(1, 1);
    
    // Get neighbor cell information
    vec4 neigTexel = GetCellTexel(texCoord + shift);
    ivec2 neigInfo = GetCellInfo(neigTexel);

    // Determine the type of the selected cell
    int colorCount = lastColorIndex + 1;
    int predator = int(texelFetch(feedRule, int(round(cellInfo.x * colorCount + neigInfo.x))).x);
    int pray = int(texelFetch(feedRule, int(round(neigInfo.x * colorCount + cellInfo.x))).x);

    ivec2 resultInfo;
    if (predator == 1) {
        outColor = vec4(1, 0, 0, 1);
        return;
        if (cellInfo.y > 0)
            resultInfo = ivec2(cellInfo.x, cellInfo.y - 1);
        else
            resultInfo = ivec2(neigInfo.x, neigInfo.y);
    } else if (pray == 1 || cellInfo.x == neigInfo.x) {
        if (cellInfo.y < maxLives)
            resultInfo = ivec2(cellInfo.x, cellInfo.y + 1);
        else
            resultInfo = cellInfo;
    }

    outColor = vec4(float(resultInfo.x) / lastColorIndex, float(resultInfo.y) / maxLives, 0, 1);
}