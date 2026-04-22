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

    return true;
}

// 해당 메서드는 루프문에서 돌아감
void Context::Render() 
{
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

    m_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) * // y축 기준으로 yaw 각도 만큼 회전
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) * // x축 기준으로 pitch 각도 만큼 회전
    glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); // z축 방향, 동차좌표계에 1을 집어넣으면 점, 0을 집어넣으면 벡터

    // 종횡비 4:3, 세로화각(FOV) 45도의 원근 투영, 원근 투영 시작점 0.01f, 끝점 10.0f 
    auto projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 30.0f);

    // 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
    // auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    // 카메라가 바라보는 타겟의 위치는 카메라 position에서 타겟의 방향벡터 더해주면 됨
    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

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

// 키보드 입력했을 때 카메라 Position값 수정하는 함수
void Context::ProcessInput(GLFWwindow* window) 
{
    // 우클릭을 통한 카메라 상호작용중이 아니라면 리턴
    if (!m_cameraControl)
    {
        return;
    }

    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cameraPos += cameraSpeed * m_cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cameraPos -= cameraSpeed * m_cameraFront;
    }

    auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_cameraPos += cameraSpeed * cameraRight;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_cameraPos -= cameraSpeed * cameraRight;    
    }

    auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_cameraPos += cameraSpeed * cameraUp;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_cameraPos -= cameraSpeed * cameraUp;
    }
}

void Context::Reshape(int width, int height) 
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

// 마우스가 움직인 좌표 x, y 인자값으로 받아 Pitch와 Yaw값 바꿔주기
void Context::MouseMove(double x, double y) 
{
    // 우클릭을 통한 카메라 상호작용중이 아니라면 리턴
    if (!m_cameraControl)
    {
        return;
    }

    // 현재 Pos값
    auto pos = glm::vec2((float)x, (float)y);

    // 현재 Pos값 - 이전 Pos값 = 현재 얼마만큼 이동했는지의 변화값
    auto deltaPos = pos - m_prevMousePos;

    // 회전 Speed
    const float cameraRotSpeed = 0.8f;

    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    // 0~360으로 Clamping 하기 위한 if문들
    if (m_cameraYaw < 0.0f)
    {
        m_cameraYaw += 360.0f;
    }   

    if (m_cameraYaw > 360.0f)
    {
        m_cameraYaw -= 360.0f;
    }

    if (m_cameraPitch > 89.0f) 
    {
        m_cameraPitch = 89.0f;
    }

    if (m_cameraPitch < -89.0f) 
    {
        m_cameraPitch = -89.0f;
    }

    // 이전 Pos값 갱신
    m_prevMousePos = pos;    
}

void Context::MouseButton(int button, int action, double x, double y) 
{
    // 입력의 종류가 마우스 우측버튼이라면
    if (button == GLFW_MOUSE_BUTTON_RIGHT) 
    {
        // 버튼을 누른것이라면
        if (action == GLFW_PRESS) 
        {
            // 마우스 조작 시작 시점에 현재 마우스 커서 위치 저장
            m_prevMousePos = glm::vec2((float)x, (float)y); // 저장 및 초기화의 역할도 수행
            m_cameraControl = true;
        }

        // 버튼을 누르지 않았다면
        else if (action == GLFW_RELEASE) 
        {
            m_cameraControl = false;
        }
    }
}