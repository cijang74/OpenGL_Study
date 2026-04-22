#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) 
{
    auto program = ProgramUPtr(new Program());

    if (!program->Link(shaders))
    {
        return nullptr;
    }
    return std::move(program);
}

bool Program::Link(const std::vector<ShaderPtr>& shaders) 
{
    m_program = glCreateProgram(); // glCreateProgram()으로 새로운 OpenGL program object 생성

    // 이터레이터임
    for (auto& shader: shaders)
    {
        glAttachShader(m_program, shader->Get()); // glAttachShader()로 program에 shader를 붙이기
    }
    glLinkProgram(m_program); // glLinkProgram()으로 program 링크

    int success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success); // glGetProgramiv()로 프로그램 링크 상태 확인

    if (!success) 
    {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog); // glGetProgramInfoLog()로 에러 로그 가져오기
        SPDLOG_ERROR("failed to link program: {}", infoLog);
        return false;
    }
    return true;
}

Program::~Program() 
{
    if (m_program) 
    {
        glDeleteProgram(m_program);
    }
}

// vertex array가 바인딩된 프로그램을 가져와 vertex array를 통해 그림 그리기
void Program::Use() const 
{
    glUseProgram(m_program);
}

// unitform이 int일때 사용할 세팅 변수
void Program::SetUniform(const std::string& name, int value) const 
{
    // 쉐이더 코드 내 uniform 변수 위치 받아와서
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(loc, value); // 값 설정
}

// unitform이 mat4일때 사용할 세팅 변수
void Program::SetUniform(const std::string& name, const glm::mat4& value) const 
{
    auto loc = glGetUniformLocation(m_program, name.c_str());

    // 인자값: uniform 위치, 매트릭스 개수, Transpose가 되어있는지, uniform에 넣어줄 값
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, float value) const 
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1f(loc, value);
}

void Program::SetUniform(const std::string& name, const glm::vec3& value) const 
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

ProgramUPtr Program::Create(const std::string& vertShaderFilename, const std::string& fragShaderFilename) 
{
    ShaderPtr vs = Shader::CreateFromFile(vertShaderFilename, GL_VERTEX_SHADER);
    ShaderPtr fs = Shader::CreateFromFile(fragShaderFilename, GL_FRAGMENT_SHADER);

    if (!vs || !fs)
    {
        return nullptr;
    }

    return std::move(Create({vs, fs}));
}

void Program::SetUniform(const std::string& name, const glm::vec4& value) const 
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(value));
}