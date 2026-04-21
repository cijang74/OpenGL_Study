#include "context.h"
#include "image.h"

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
    float vertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // x, y, z, r, g, b, s, t
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    // 정점의 index로 구성된 array 준비
    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // 0, 1, 3 점 이어서 만드려는 삼각형 하나
        1, 2, 3, // 1, 2, 3 점 이어서 만드려는 삼각형 하나
    };

    // 무조건 VAO를 먼저 만들어 준 뒤 VBO를 만들어야 한다.
    m_vertexLayout = VertexLayout::Create();

    // VBO 만들기
    // GL_ARRAY_BUFFER: 사용할 buffer object는 vertex data를 저장할 용도임을 알림 (위치, 색상값으로 사용할 버퍼임을 알림)
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 32);
    
    // // VAO와 VBO 연결
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3); // 추가. Color가 시작되는 부분은 sizeof(float) * 3이후부터 이므로 offset 지정
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer); // 지금부터 사용할 buffer object를 지정 -> m_vertexBuffer는 GL_ELEMENT_ARRAY_BUFFER 용도로 쓸거야.
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6); // 벡터가 아닌, 정수값들만 읽어오면 되기에 VAO 작성 필요 X

    // shader.h, shader.cpp에 작성된 함수들은 glad 함수를 사용하기 때문에 초기화 이후 사용 가능
    // 쉐이더 인스턴스 생성
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);

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

    // uniform 입력 과정
    // auto loc = glGetUniformLocation(m_program->Get(), "color"); // 프로그램 내 color 변수 핸들(int)을 얻는다.
    // m_program->Use(); // 현재 사용할 프로그램으로 등록 후 (해당 프로그램을 가지고 그림을 그리겠다 선언)
    // glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f); // 위에서 얻은 변수 핸들을 통해 값을 전달한다.

    // 화면을 지우려고 할 때 어떤 색으로 지울지 세팅
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

    // 이미지 불러오기
    auto image = Image::Load("./image/container.jpg");

    if (!image)
    {
        return false;
    }
    SPDLOG_INFO("image: {}x{}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());

    // 텍스쳐 생성하여 바인딩
    m_texture = Texture::CreateFromImage(image.get());

    // 두 번째 이미지 로드하여 텍스쳐 생성하여 바인딩
    auto image2 = Image::Load("./image/awesomeface.png");
    m_texture2 = Texture::CreateFromImage(image2.get());

    // 내가 지금부터 사용할 텍스쳐 슬롯 번호는 0번입니다.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get()); // 0번 슬롯에 사용하려는 GL_TEXTURE_2D 형태의 텍스쳐 객체 id를 준비해놓을게요.

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

    m_program->Use(); // 해당 프로그램 사용할건데
    
    // glUniform1i: 프로그램 내 tex라는 uniform변수를 가져와서 텍스쳐 슬롯 0번에 있는 텍스쳐 객체 id 연결
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
    glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);

    return true;
}

// 해당 메서드는 루프문에서 돌아감
void Context::Render() 
{
    glClear(GL_COLOR_BUFFER_BIT); // glClearColor로 세팅한 색으로 색상 버퍼 초기화

    // static float time = 0.0f;
    // float t = sinf(time) * 0.5f + 0.5f;
    // auto loc = glGetUniformLocation(m_program->Get(), "color");

    // vertex array가 바인딩된 프로그램을 가져와 vertex array를 통해 그림 그리기
    m_program->Use();

    // glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 6은 정점 index의 개수, 인덱스 자료형 unsignedint, 첫 정점의 index
    // glDrawArrays(GL_TRIANGLES, 0, 6); // 현재 설정된 program, VBO, VAO로 그림을 그린다. (어떤 VBO? 어떤 VAO? -> 현재 바인딩 되어있는 VBO와 VAO)
    // GL_TRIANGLES: 현재 그리고자 하는 promitive 타입, offset: 그리고자 하는 첫 정점의 index, count: 그리려는 정점의 총 개수

    // time += 0.016f;
}