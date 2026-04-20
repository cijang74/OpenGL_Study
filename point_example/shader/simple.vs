#version 330 core // 3.3 버전 이상부터 작동 명시
layout (location = 0) in vec3 aPos;

// 쉐이더 코드도 GPU 코어 하나하나에서 실행하는 하나의 코드이므로 main함수를 작성해야 함.
void main() 
{
    // 가장 단순한 vertex shader 작성: vertex shader는 정점의 위치를 설정하는 쉐이더
    gl_Position = vec4(0.5, 0.5, 0.0, 1.0); // x, y, z, 동차 좌표계 w(원근감 및 이동 계산용 좌표)
}