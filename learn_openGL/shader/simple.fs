#version 330 core

uniform vec4 color; // uniform변수로 선언되어, application딴에서 접근 가능하게되고, 해당 fs를 사용하는 모든 프로그램이 해당 변수를 사용하게 됨.
out vec4 fragColor;

void main() 
{
    fragColor = color;
}