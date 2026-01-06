#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec2 texCoord;
out vec3 normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 viewPos;

void main()
{
    texCoord = aTexCoord;
    normal = normalize(mat3(transpose(inverse(modelMat))) * aNormal);

    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
}