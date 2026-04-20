#include "context.h"

ContextUPtr Context::Create() 
{
    auto context = ContextUPtr(new Context());

    if (!context->Init())
    {
        return nullptr;
    }
    return std::move(context);
}

// src/main.cpp에서 테스트하던 코드를 가지고 오기
bool Context::Init() 
{
    // shader.h, shader.cpp에 작성된 함수들은 glad 함수를 사용하기 때문에 초기화 이후 사용 가능
    // 쉐이더 인스턴스 생성
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);

    if (!vertShader || !fragShader)
    {
        return false;
    }

    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

    // 앞에서 생성한 쉐이더 인스턴스를 가지고 프로그램 인스턴스 생성
    m_program = Program::Create({fragShader, vertShader});

    if (!m_program)
    {
        return false;
    }
    SPDLOG_INFO("program id: {}", m_program->Get());

    // 화면을 지우려고 할 때 어떤 색으로 지울지 세팅
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    // Context::Init()에서 vertex array object를 생성
    // vertex array를 만들고 바인딩
    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return true;
}

void Context::Render() 
{
    glClear(GL_COLOR_BUFFER_BIT); // glClearColor로 세팅한 색으로 색상 버퍼 초기화

    // vertex array가 바인딩된 프로그램을 가져와 vertex array를 통해 그림 그리기
    glUseProgram(m_program->Get());
    glDrawArrays(GL_POINTS, 0, 1);
}