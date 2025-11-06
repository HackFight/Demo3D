#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 vertexColor;
in vec3 fragPos;

struct Material {
    sampler2D   texture_diffuse1;
    sampler2D   texture_specular1;
    sampler2D   texture_emission1;
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
uniform bool gamma;

void main()
{
    // ambient
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, texCoord));

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, texCoord));  

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));

    // emission
    vec3 emission = texture(material.texture_emission1, texCoord).rgb;

    // simple attenuation
    float distance = 1.5;
    float attenuation = 1.0 / (gamma ? distance * distance : distance);

    FragColor = vec4(ambient + (diffuse + specular) * attenuation + emission, 1.0);
}