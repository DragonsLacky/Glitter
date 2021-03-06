#version 330 core
#define NUM_LIGHTS 128

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

const int Directional = 0;
const int Spotlight = 2;
const int PointLight = 1;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light{
    int type;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 color;
};


uniform vec3 viewPos;
uniform Light Lights[NUM_LIGHTS];
uniform Material material;
uniform int size;
uniform bool gamma;
uniform bool light;
uniform float far_plane;
uniform samplerCube depthMap;
uniform sampler2D depthMapD;
uniform vec3 lp;
uniform vec3 lghtColor;
uniform int l;
uniform vec3 lightPos;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// function prototypes
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 BlinnPhong(vec3 normal, vec3 fragPos, Light light, vec3 color, vec3 specMap);
float ShadowCalculation(vec3 fragPos);
float ShadowCalcDir(vec4 fragPosLightSpace);
void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 result =  vec3(texture(material.diffuse, TexCoords).rgb);

    vec3 color = texture(material.diffuse, TexCoords).rgb;
    vec3 specmap = texture(material.specular, TexCoords).rgb;
    vec3 lighting = vec3(0.0f);

    for (int i = 0; i < size; ++i)
    {
        lighting += BlinnPhong(norm, FragPos, Lights[i], color, specmap);
    }
    if (gamma)
    {
       lighting = pow(lighting, vec3(1.0f/ 2.2f));
    }
    FragColor = vec4(lighting, 1.0f);
    if (light)
    {
        FragColor= vec4(lghtColor*255,1.0f);
    }
}

vec3 BlinnPhong(vec3 normal, vec3 fragPos, Light light, vec3 color, vec3 specMap)
{
    

    vec3 ambient = light.ambient * color;

    vec3 lightDir = vec3(0.0f);
    if (light.type == Directional)
    {
        lightDir = normalize(-light.direction);
    }
    else 
    {
        lightDir = normalize(light.position - fragPos);
    }
    // diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.color;
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64);
    vec3 specular = spec * light.color;    
    // simple attenuation
    float max_distance = 1.5;
    float distance = 0;
    if (light.type != Directional)
    {
        distance = length(light.position - fragPos);
    }
    float attenuation = 1.0 /(light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance)); //(light.constant + (gamma ? light.quadratic * distance * distance : light.linear * distance));
    float intensity = 1.0f;
    if(light.type == Spotlight)
    {
        float theta = dot(lightDir, normalize(-light.direction)); 
        float epsilon = light.cutOff - light.outerCutOff;
        intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        intensity *= 10;
        ambient *= intensity;
    }
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    float shadow = ShadowCalculation(fragPos);
    //shadow += ShadowCalcDir(FragPosLightSpace);
    //shadow = clamp(shadow, 0.0,1.0);

    return (ambient +(1.0 - shadow) * diffuse)*color + ((1.0 - shadow) * specular)*specMap;
}


float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - Lights[l].position;
    float currentDepth = length(fragToLight);
    //float closestDepth = texture(depthMap, fragToLight).r;
    //closestDepth *= far_plane;


    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
       
    //FragColor = vec4(vec3(closestDepth), 1.0);    

    return shadow;
}

float ShadowCalcDir(vec4 fragPosLightSpace)
{
    vec3 coords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    coords = coords * 0.5 + 0.5;
    float closestDepth = texture(depthMapD, coords.xy).r; 
    float currentDepth = coords.z;
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 specular = vec3(0.3) * spec;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}