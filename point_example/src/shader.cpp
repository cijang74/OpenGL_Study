#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) 
{
    // auto shader = std::unique_ptr<Shader>(new Shader());
    auto shader = ShaderUPtr(new Shader());

    if (!shader->LoadFile(filename, shaderType))
    {
        return nullptr;
    }

    // 포인터 소유권 이전
    return std::move(shader);
}

// 성공시 로딩된 텍스트 포인터 및 길이 가져오기
bool Shader::LoadFile(const std::string& filename, GLenum shaderType) 
{
    auto result = LoadTextFile(filename);

    if (!result.has_value())
    {
        return false;
    }

    auto& code = result.value();
    const char* codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    // 쉐이더를 만들고 컴파일
    m_shader = glCreateShader(shaderType); // 쉐이더 정보를 넘겨받아 쉐이더 객체 생성 -> 쉐이더 핸들 번호 반환
    glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength); // 쉐이더 정보와 코드 개수, 코드의 포인터, 코드 길이를 넘겨줌
    glCompileShader(m_shader); // glShaderSource를 통해 넣어준 코드를 컴파일

    // check compile error
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success); // 쉐이더에 있는 정보(GL_COMPILE_STATUS) 읽어오기

    if (!success) 
    {
        char infoLog[1024]; // 버퍼 선언
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("reason: {}", infoLog);
        return false;
    }

    return true;
}

// 소멸자 구현
Shader::~Shader() 
{
    // m_shader는 처음에 0으로 초기화 -> 0이 아닌 다른 값이 들어가 있다면 뭔가 할당을 받았다는 뜻
    if (m_shader) 
    {
        glDeleteShader(m_shader); // glDeleteShader()를 호출하여 shader object 제거
    }
}