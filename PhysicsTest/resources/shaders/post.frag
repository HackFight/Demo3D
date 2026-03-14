#version 330 core
out vec4 fragColor;
  
in vec2 texCoord;

struct Material {
    sampler2D   texture_diffuse1;
};

uniform Material material;

uniform bool gammaCorrection;
uniform bool toneMapping;
uniform float exposure;

void main()
{ 
    vec3 color = texture(material.texture_diffuse1, texCoord).rgb;
    if (toneMapping) color = vec3(1.0) - exp(-color * exposure);
    if (gammaCorrection) color = pow(color, vec3(1.0/2.2));
    
    fragColor = vec4(color, 1.0);
}