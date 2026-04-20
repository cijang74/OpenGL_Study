#include "context.h"
// #include <spdlog/spdlog.h>
// #include <glad/glad.h> // 주의: 반드시 #include <GLFW/glfw3.h> 이전에 추가할 것. 안그러면 오류 생김
// #include <GLFW/glfw3.h>

// 윈도우 프레임버퍼 크기가 변경되었을 때 호출 할 콜백함수 정의
void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) 
{
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height); // glViewport: OpenGL이 그림을 그릴 영역 지정
}

// 키보드 입력이 이루어졌을 때 호출 할 콜백 함수 정의
void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    // 로그 찍기
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");

    // ESC키라면 닫기
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char** argv) 
{
    // 시작을 알리는 로그
    SPDLOG_INFO("Start program");
    
    // glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) // 초기화 실패하면
    {
        const char* description = nullptr; // 에러사유 저장할 변수
        glfwGetError(&description); // 변수에 에러사유 저장
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    // glfw에서 윈도우를 생성하기 전에, OpenGL 어떤 버전을 사용할지, 어떤 PROFILE을 사용할지 힌트를 준다.
    // Windows 환경에서는 해당 라인들이 없어도 최적화가 구린 호환성 프로필이라는 관대한 상태로 OpenGL을 세팅하기에 문제가 없지만, 다른 OS에서는 문제가 생길 수 있다.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 및 OpenGLContext생성, 실패하면 에러 출력후 종료
    // OpenGL Function들은 어떤 Window의 Context에 그림을 그려야 하는지 따로 세팅할 수 없다. 그냥 지금 현재 대상이 되는 Context에 그림을 그릴 뿐.
    SPDLOG_INFO("Create glfw window");
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr); // 윈도우 생성

    if (!window) // 윈도우 생성 실패하면
    {
        SPDLOG_ERROR("failed to create glfw window"); // 에러 메시지 띄우고
        glfwTerminate(); // 메모리 정리 후 종료
        return -1;
    }

    // 28번째 라인에서 window생성하며 만든 Context를 앞으로 주 Context로 사용하겠다고 설정, 즉 그림 그릴려면 해당 과정 필요
    glfwMakeContextCurrent(window);

    // OpenGL Context 생성 이후(= Window 생성 이후) 실행할 것. glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // 함수 로딩 실패하면
    {
        SPDLOG_ERROR("failed to initialize glad"); // 에러 메시지 띄우고
        glfwTerminate(); // 메모리 정리 후 종료
        return -1;
    }

    // glad를 활용한 OpenGL 함수 로딩이 성공하면 OpenGL 함수들 사용 가능
    auto glVersion = glGetString(GL_VERSION); // 예시로 테스트 해볼 OpenGL함수. 버전을 확인하여 glVersion변수에 저장
    SPDLOG_INFO("OpenGL context version: {}", (const char*)glVersion); // 버전 출력

    // context 객체 생성 (context.h와 context.cpp에 작성된 함수들은 glad 함수를 사용하기 때문에 초기화 이후 사용 가능)
    auto context = Context::Create();
    if (!context) 
    {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    // 콜백함수 바인딩
    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT); // 프레임버퍼 변경 이벤트가 첫 생성 직후에는 발생하지 않으므로 수동 호출 -> 그림 그릴 영역 지정
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료. 루프를 돌아야 창이 생성되고 나서 바로 종료되는 현상이 발생 안함.
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) // 윈도우를 닫아야 한다는 신호를 받기 전까지는 계속 루프를 돈다
    {
        glfwPollEvents(); // 윈도우에서 입력받은 이벤트 (키보드, 마우스 등)..을 감지
        // glClearColor(0.1f, 0.2f, 0.3f, 0.0f); // 화면을 지우려고 할 때 어떤 색으로 지울지 세팅
        // glClear(GL_COLOR_BUFFER_BIT); // 백 버퍼 화면 지우기 (교체 전까지 뭔가 그린다는 것 -> 백버퍼에 그린다는 것)
        context->Render();
        glfwSwapBuffers(window); // 백 버퍼와 프론트 버퍼 교체
    }

    context.reset(); // context 객체 내부에서 사용하던 context, shader, program 메모리들 정리
    glfwTerminate(); // 종료되면 메모리 정리 후 종료
    return 0;
}