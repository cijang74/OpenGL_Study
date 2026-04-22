#version 330 core

// attribure pointer로부터 넘겨받는 인자값
layout (location = 0) in vec3 aPos; // 위치
layout (location = 1) in vec3 aNormal; // 노말 벡터
layout (location = 2) in vec2 aTexCoord; // 텍스쳐

// 유니폼 변수
uniform mat4 transform; // 확대, 회전, 이동에 대한 행렬을 유니폼으로 받음 (MVP 행렬)
uniform mat4 modelTransform; // 로컬 좌표를 월드 좌표로 변환시켜주는 Model Metrix

// 출력값
out vec3 normal;
out vec2 texCoord;
out vec3 position;

void main() 
{
    gl_Position = transform * vec4(aPos, 1.0); // 화면상에 어디에 있는지의 좌표 (canonical space 상의 좌표값)
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz; // 노멀 벡터 계산 (월드 좌표 상에서 표면의 normal 방향)
    // normal에 modelTransform의 inverse transpose를 적용하는 이유
    // 점이 아닌 벡터의 경우 이렇게 해야 제대로 변환된 값을 계산할 수 있음 (안하면 스케일링 이후 노말 벡터가 찌그러질 수 있음)
    // matrix의 inverse transpose는 모든 점에서 동일하므로 보통 별도의 uniform으로 입력함

    texCoord = aTexCoord;
    position = (modelTransform * vec4(aPos, 1.0)).xyz; // 월드 좌표 상에서 어디에 있는지의 좌표
    // position을 계산하여 fragment shader에 넘기는 이유
    // gl_Position은 perspective transform이 적용되어 있어 canonical space 상의 좌표값으로 전환됨
    // diffuse 값을 계산하려면 world space 상에서의 좌표값이 필요
    // 따라서 world space상에서의 좌표값으로 쓸 position을 계산하여 넘겨줌
}