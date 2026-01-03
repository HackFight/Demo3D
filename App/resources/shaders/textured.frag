#version 330 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 vertexColor;
in vec3 fragPos;
in vec4 fragPosLightSpace;

struct Material {
    sampler2D   texture_diffuse1;
    sampler2D   texture_specular1;
    sampler2D   texture_emission1;
    sampler2D   texture_normal1;
    float       shininess;
};

struct Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    float bias = 0.0005;
    //float shadow = step(texture(shadowMap, projCoords.xy).r, projCoords.z - bias);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    vec4 diffuseSample = texture(material.texture_diffuse1, texCoord);
    if(diffuseSample.a < 0.1) discard;

    // ambient
    vec3 ambient  = light.ambient  * diffuseSample.rgb;

    // diffuse
    vec3 norm = texture(material.texture_normal1, texCoord).rgb;
    norm = normalize(normal * 2.0 - 1.0);

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * diffuseSample.rgb;  

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, texCoord).r;

    // emission
    vec3 emission = texture(material.texture_emission1, texCoord).rgb;

    // calculate shadow
    float shadow = ShadowCalculation(fragPosLightSpace);

    fragColor = vec4((ambient + (1.0 - shadow) * (diffuse + specular)) + emission, 1.0);
}