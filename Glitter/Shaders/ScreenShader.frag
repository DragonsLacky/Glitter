#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 blurTexttureCoordsHorizontal[11];
in vec2 blurTexttureCoordsVertical[11];

uniform vec2 tlMenuCoords[128];
uniform vec2 brMenuCoords[128];
uniform int size;

uniform sampler2D screenTexture;
uniform bool blur;

bool checkForMenu(vec2 fragcoord, vec2[128] tlmc, vec2[128] brmc, int size);

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * 1.0 * 7.5) / (7.5 + 1.0 - z * (7.5 - 1.0));	
}

void main()
{        
    const float gamma = 2.2f;
    vec2 fc = vec2(2*gl_FragCoord.x/800 - 1, 2 * gl_FragCoord.y/800 - 1);
    bool menu = checkForMenu(fc, tlMenuCoords, brMenuCoords, size);
    if(blur && !menu)
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

        FragColor = vec4(pow(vec3(1.0f) -  exp((-FragColor.rgb) * 0.2 ),vec3(1.0f / gamma)),1.0f);
        //vec4(vec3(1.0f) - exp(-(FragColor.rbg) * 1.0f), 1.0f);
        FragColor = FragColor.a * FragColor + (1 - FragColor.a ) *  FragColor.rgba;

    }
    else{
        vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
        vec3 mapped = vec3(0.0f);
        if(blur && menu)
        {
            mapped =  vec3(1.0f) -  exp(-hdrColor * 1.0f );
        }
        else
        {
            mapped =  vec3(1.0f) -  exp(-hdrColor * 0.6f );
        }
        //hdrColor / (hdrColor + vec3(1.0f));
        //mapped = pow(mapped, vec3(1.0 / gamma));

        FragColor = vec4(mapped, 1.0f);
        FragColor = FragColor.a * FragColor + (1 - FragColor.a ) *  FragColor.rgba;


        //float depthValue = texture(screenTexture, TexCoords).r;
        //FragColor = vec4(vec3(LinearizeDepth(depthValue) / 7.5), 1.0); // perspective
        //FragColor = vec4(vec3(depthValue), 1.0); // orthographic
    }
    
} 

bool checkForMenu(vec2 fragcoord, vec2[128] tlmc, vec2[128] brmc, int size)
{
    for (int i = 0; i < size; i++)
    {
        if(fragcoord.x > tlmc[i].x && fragcoord.y < tlmc[i].y && fragcoord.x < brmc[i].x && fragcoord.y > brmc[i].y)
        {
            return true;
        }
    }
    return false;
}
