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