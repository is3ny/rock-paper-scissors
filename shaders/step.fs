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
uniform sampler2D randomMap;

uniform mat4 toNDC;
uniform mat4 ndcToTex;

uniform sampler2D automata;

// 2D Random
float Random (in vec2 st) {
    vec2 seededCoord = st + seed;
    vec2 txcoord = (ndcToTex * toNDC * vec4(seededCoord, 0, 1)).xy;
    return texture(randomMap, txcoord).x;
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
    int choice = int(round(Random(texCoord) * (dsSize - 1)));
    vec2 shift = 2 * texelFetch(ds, choice).xy - vec2(1, 1);

    // Get neighbor cell information
    vec4 neigTexel = GetCellTexel(texCoord + shift);
    ivec2 neigInfo = GetCellInfo(neigTexel);

    // Determine the type of the selected cell
    int colorCount = lastColorIndex + 1;
    int predator = int(texelFetch(feedRule, neigInfo.x * colorCount + cellInfo.x).x);
    //int pray = int(texelFetch(feedRule, cellInfo.x * colorCount + neigInfo.x).x);

    //if (predator == 1 || pray == 1)

    ivec2 resultInfo;
    if (predator == 1 && cellInfo.y == 0) {
        resultInfo = ivec2(neigInfo.x, maxLives);
    } else if (cellInfo.y > 0) {
        resultInfo = ivec2(cellInfo.x, cellInfo.y - 1);
    }

    outColor = vec4(float(resultInfo.x) / lastColorIndex, float(resultInfo.y) / maxLives, 0, 1);
}