#version 330 core

in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

// 외부에서도 접근 가능한 모든 텍스쳐 쉐이더가 공유하는 전역변수
uniform sampler2D tex; // sampler2D: 쉐이더 안에서는 2D텍스쳐 타입을 이렇게 표현함
uniform sampler2D tex2; // 2번째 텍스쳐 사용할 변수

void main() 
{
    fragColor = texture(tex, texCoord) * 0.8 + texture(tex2, texCoord) * 0.2;
}