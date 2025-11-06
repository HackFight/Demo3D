#version 330 core
out vec4 fragColor;
  
in vec2 texCoord;

struct Material {
    sampler2D   texture_diffuse1;
};

uniform Material material;

void main()
{ 
    fragColor = vec4(vec3(1.0 - texture(material.texture_diffuse1, texCoord)), 1.0);
}