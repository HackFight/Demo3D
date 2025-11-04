#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 vertexColor;
in vec3 fragPos;

struct Material {
    sampler2D   diffuse;
    sampler2D   specular;
    sampler2D   emission;
    float       shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;

uniform vec3 viewPos;

void main()
{
    // ambient
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoord));

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoord));  

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

    // emission
    vec3 emission = texture(material.emission, texCoord).rgb;

    FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}