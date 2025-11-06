#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec2 texCoord;
out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);

    texCoord = aTexCoord;
    vertexColor = aColor;
}