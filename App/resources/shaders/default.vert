#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
  
out vec3 vertexColor;
out vec3 fragPos;
out vec3 normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);

    vertexColor = aColor;
    fragPos = vec3(modelMat * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(modelMat))) * aNormal;
}