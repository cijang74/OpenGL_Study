#version 330 core

// Vertex Shader로부터 넘겨받는 인자값
in vec3 normal; // 노말벡터
in vec2 texCoord; // 텍스쳐 좌표
in vec3 position;

// 출력값 (필수)
out vec4 fragColor;

// 유니폼 변수
uniform vec3 viewPos; // 시선의 위치 (보고있는 눈의 위치)

// 빛
struct Light 
{
    vec3 position; // 광원 위치
    vec3 ambient; // 주변광
    vec3 diffuse; // 분산광
    vec3 specular; // 반사광
};
uniform Light light;

// 물체의 머티리얼
struct Material 
{
    sampler2D diffuse; // 머티리얼 텍스쳐 색상
    sampler2D specular; // 반사광
    float shininess; // 반사광의 면적
};
uniform Material material;

// uniform vec3 lightPos;
// uniform vec3 lightColor; // 광원 색
// uniform vec3 objectColor; // 오브젝트 색
// uniform float ambientStrength; // 주변광 량

// uniform float specularStrength; // 반사광의 정도
// uniform float specularShininess; // 반사광의 면적

void main() 
{
    vec3 texColor = texture2D(material.diffuse, texCoord).xyz; // 텍스쳐 색상
    vec3 ambient = texColor * light.ambient; // 텍스쳐 색상까지 고려하여 ambient 계산

    // 다시 normalize 하는 이유: vs에서 계산된 값은 rasterization 과정에서 보간이 진행되어 nomalization 보장이 안됨.
    vec3 lightDir = normalize(light.position - position); // 빛의 방향 계산 -> 빛의 위치와 픽셀의 월드 좌표계 위치의 차
    vec3 pixelNorm = normalize(normal); // 물체 표면의 법선 벡터 방향

    // max를 통해 0보다 작으면 그냥 0을 쓰게끔 설정
    float diff = max(dot(pixelNorm, lightDir), 0.0);
    vec3 diffuse = diff * texColor * light.diffuse; // 분산광 계산

    vec3 specColor = texture2D(material.specular, texCoord).xyz; // 텍스쳐 색상
    vec3 viewDir = normalize(viewPos - position); // 시선의 방향 -> 시선의 위치와 픽셀의 월드 좌표계 위치의 차

    // reflect: light 벡터 방향의 광선이 normal 벡터 방향의 표면에 부딪혔을 때 반사되는 벡터를 출력하는 내장함수
    vec3 reflectDir = reflect(-lightDir, pixelNorm); // 반사 방향 -> reflect라는 함수에 인자값으로 빛의 방향벡터 * -1값과 픽셀의 법선벡터를 넘겨주면 됨
    
    // spec은 반사광이 만드는, 오브젝트의 흰색 영역
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // pow는 지수함수로, specularShininess가 커질수록 반사광이 만드는 흰색 영역이 작아지게 함
    vec3 specular = spec * specColor * light.specular; // 반사광 계산

    vec3 result = ambient + diffuse + specular; // 주변광 + 분산광 + 반사광을 모두 고려한 결과
    fragColor = vec4(result, 1.0);
}