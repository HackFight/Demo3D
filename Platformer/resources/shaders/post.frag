#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texUnit;

void main()
{
    fragColor = vec4(texture(texUnit, texCoord));

    float gamma = 2.2;
    fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
}