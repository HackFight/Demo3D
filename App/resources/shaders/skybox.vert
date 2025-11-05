#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoord;

uniform mat4 projMat;
uniform mat4 viewMat;

void main()
{
    texCoord = aPos;
    vec4 pos = projMat * viewMat * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}