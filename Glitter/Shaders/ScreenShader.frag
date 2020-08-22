#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 blurTexttureCoordsHorizontal[11];
in vec2 blurTexttureCoordsVertical[11];

uniform sampler2D screenTexture;
uniform bool blur;
void main()
{        
    const float gamma = 2.2f;

    if(blur)
    {
        FragColor = vec4(0.0);
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[0]) * 0.0093;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[1]) * 0.028002;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[2]) * 0.065984;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[3]) * 0.121703;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[4]) * 0.175713;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[5]) * 0.198596;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[6]) * 0.175713;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[7]) * 0.121703;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[8]) * 0.065984;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[9]) * 0.028002;
        FragColor += texture(screenTexture, blurTexttureCoordsHorizontal[10]) * 0.0093;

        FragColor += texture(screenTexture, blurTexttureCoordsVertical[0]) * 0.0093;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[1]) * 0.028002;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[2]) * 0.065984;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[3]) * 0.121703;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[4]) * 0.175713;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[5]) * 0.198596;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[6]) * 0.175713;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[7]) * 0.121703;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[8]) * 0.065984;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[9]) * 0.028002;
        FragColor += texture(screenTexture, blurTexttureCoordsVertical[10]) * 0.0093;

        FragColor = vec4(pow(vec3(1.0f) -  exp(-FragColor.rgb * 0.2 ),vec3(1.0f / gamma)),1.0f);
        //FragColor = FragColor.a * FragColor + (1 - FragColor.a ) *  FragColor.rgba;

    }
    else{
        vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

        vec3 mapped =  vec3(1.0f) -  exp(-hdrColor * 3.0f );//hdrColor / (hdrColor + vec3(1.0f));
        mapped = pow(mapped, vec3(1.0 / gamma));

        FragColor = vec4(mapped, 1.0f);
        FragColor = FragColor.a * FragColor + (1 - FragColor.a ) *  FragColor.rgba;
    }
    
} 