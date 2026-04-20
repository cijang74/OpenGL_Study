#version 330 core
out vec4 fragColor;

void main() 
{
    // 가장 단순한 fragment shader 작성: fragment shader는 정점의 색을 설정하는 쉐이더
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}