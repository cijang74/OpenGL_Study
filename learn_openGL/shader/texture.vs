#version 330 core

layout (location = 0) in vec3 aPos; // 위치값
layout (location = 1) in vec3 aColor; // rgb값
layout (location = 2) in vec2 aTexCoord; // texture coordinate값

out vec4 vertexColor;
out vec2 texCoord;

void main() 
{
    gl_Position = vec4(aPos, 1.0); // 1은 동차좌표계 w 
    vertexColor = vec4(aColor, 1.0); // 1은 alpha값
    texCoord = aTexCoord;
}