#version 330 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;

uniform sampler2D texUnit;

void main()
{
    fragColor = vec4(texture(texUnit, texCoord));
}