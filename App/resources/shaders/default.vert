#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec2 texCoord;
out vec3 normal;
out vec3 vertexColor;
out vec3 fragPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);

    texCoord = aTexCoord;
    normal = mat3(transpose(inverse(modelMat))) * aNormal;
    vertexColor = aColor;
    fragPos = vec3(modelMat * vec4(aPos, 1.0));
}