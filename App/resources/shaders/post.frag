#version 330 core
out vec4 fragColor;
  
in vec2 texCoord;

struct Material {
    sampler2D   texture_diffuse1;
};

uniform Material material;

uniform bool toneMapping;
uniform float exposure;

void main()
{ 
    vec3 hdrColor = texture(material.texture_diffuse1, texCoord).rgb;

    // reinhard tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    if (toneMapping)
    {
        fragColor = vec4(mapped, 1.0);
    }
    else
    {
        fragColor = vec4(hdrColor, 1.0);
    }
}