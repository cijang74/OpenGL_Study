#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

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

    // glfw 윈도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr); // 윈도우 생성

    if (!window) // 윈도우 생성 실패하면
    {
        SPDLOG_ERROR("failed to create glfw window"); // 에러 메시지 띄우고
        glfwTerminate(); // 메모리 정리 후 종료
        return -1;
    }

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료. 루프를 돌아야 창이 생성되고 나서 바로 종료되는 현상이 발생 안함.
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) // 윈도우를 닫아야 한다는 신호를 받기 전까지는 계속 루프를 돈다
    {
        glfwPollEvents(); // 윈도우에서 입력받은 이벤트 (키보드, 마우스 등)..을 감지
    }

    glfwTerminate(); // 종료되면 메모리 정리 후 종료
    return 0;
}