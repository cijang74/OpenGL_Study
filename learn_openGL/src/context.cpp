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
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // // x, y, z, s, t
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
    0,  2,  1,  2,  0,  3, // 0, 2, 1, 2, 0, 3 점 이어서 하나의 면
    4,  5,  6,  6,  7,  4,
    8,  9, 10, 10, 11,  8,
    12, 14, 13, 14, 12, 15,
    16, 17, 18, 18, 19, 16,
    20, 22, 21, 22, 20, 23,
    };

    // 무조건 VAO를 먼저 만들어 준 뒤 VBO를 만들어야 한다.
    m_vertexLayout = VertexLayout::Create();

    // VBO 만들기
    // GL_ARRAY_BUFFER: 사용할 buffer object는 vertex data를 저장할 용도임을 알림 (위치, 색상값으로 사용할 버퍼임을 알림)
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 120);
    
    // // VAO와 VBO 연결
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    // m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3); // 추가. Color가 시작되는 부분은 sizeof(float) * 3이후부터 이므로 offset 지정
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer); // 지금부터 사용할 buffer object를 지정 -> m_vertexBuffer는 GL_ELEMENT_ARRAY_BUFFER 용도로 쓸거야.
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36); // 벡터가 아닌, 정수값들만 읽어오면 되기에 VAO 작성 필요 X

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
    m_program->SetUniform("tex", 0);
    m_program->SetUniform("tex2", 1);

    // x축으로 -55도 회전
    auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 로컬 스페이스 -> 월드 스페이스

    // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); // 월드 스페이스 -> 뷰 스페이스

    // 종횡비 4:3, 세로화각(FOV) 45도의 원근 투영, 원근 투영 시작점 0.01f, 끝점 10.0f 
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 10.0f); // 뷰 스페이스 -> 클립 스페이스
    auto transform = projection * view * model;
    m_program->SetUniform("transform", transform);

    return true;
}

// 해당 메서드는 루프문에서 돌아감
void Context::Render() 
{
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // glClearColor로 세팅한 색으로 색상 버퍼 초기화, 깊이 버퍼도 초기화
    // glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화
    
    // // vertex array가 바인딩된 프로그램을 가져와 vertex array를 통해 그림 그리기
    // m_program->Use();
    
    // // 종횡비 4:3, 세로화각(FOV) 45도의 원근 투영, 원근 투영 시작점 0.01f, 끝점 10.0f 
    // auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 10.0f);

    // // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    // auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    // // x축으로 매 초마다 120도, y축으로 매 초마다 60도 회전
    // auto model = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 120.0f), glm::vec3(1.0f, 0.5f, 0.0f));

    // auto transform = projection * view * model;
    // m_program->SetUniform("transform", transform);

    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36은 정점 index의 개수, 인덱스 자료형 unsignedint, 첫 정점의 index
    // // GL_TRIANGLES: 현재 그리고자 하는 promitive 타입, offset: 그리고자 하는 첫 정점의 index, count: 그리려는 정점의 총 개수

    // 큐브 소환할 위치들 지정
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    // 버퍼 초기화
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    m_program->Use();

    // 종횡비 4:3, 세로화각(FOV) 45도의 원근 투영, 원근 투영 시작점 0.01f, 끝점 10.0f 
    auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 20.0f);

    // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    for (size_t i = 0; i < cubePositions.size(); i++)
    {
        auto& pos = cubePositions[i]; // 이번에 소환할 큐브의 위치값
        auto model = glm::translate(glm::mat4(1.0f), pos); // 소환할 큐브를 위치값으로 이동
        // 큐브를 x축으로 매 초마다 140 + i도, y축으로 매 초마다 70 + i/2도 회전
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i), glm::vec3(1.0f, 0.5f, 0.0f));

        // transform 계산하여 uniform에 넣어주기
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}