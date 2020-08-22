#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 blurTexttureCoordsHorizontal[11];
out vec2 blurTexttureCoordsVertical[11];

uniform float targetWidth;
uniform float targetHeight;

void main()
{
    TexCoords = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    float pixelSizeH = 1.0f / targetWidth;
    float pixelSizeV = 1.0f / targetHeight;

    for (int i = -5; i < 5; i++ )
    {
        blurTexttureCoordsHorizontal[i+5] = TexCoords + vec2(pixelSizeH * i, 0.0f);
        blurTexttureCoordsVertical[i+5] = TexCoords + vec2(0.0f, pixelSizeV * i);
    }
}  