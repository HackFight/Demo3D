#version 330 core
out vec4 fragColor;
  
in vec2 texCoord;

struct Material {
    sampler2D   texture_diffuse1;
};

uniform Material material;
uniform bool gamma;

void main()
{ 
    fragColor = texture(material.texture_diffuse1, texCoord);
}