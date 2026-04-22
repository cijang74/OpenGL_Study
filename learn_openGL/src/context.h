#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"

// 프로그램 라이프사이클을 고려하여 코드 리팩토링
// OpenGL object 들을 관리하고 렌더링하는 코드를 분리하고자 context.h, context.cpp 작성

CLASS_PTR(Context)
class Context 
{
    public:
        static ContextUPtr Create();
        void Render();
        void ProcessInput(GLFWwindow* window);
        void Reshape(int width, int height);
        void MouseMove(double x, double y);
        void MouseButton(int button, int action, double x, double y);

    private:
        Context() {}
        bool Init();
        ProgramUPtr m_program;
        ProgramUPtr m_simpleProgram;

        // uint32_t m_vertexArrayObject; // VAO 변수 선언
        // uint32_t m_vertexBuffer; // VBO(정점 데이터의 정보)를 담아둘 변수 선언
        // uint32_t m_indexBuffer; // EBO 변수 선언

        VertexLayoutUPtr m_vertexLayout;
        BufferUPtr m_vertexBuffer;
        BufferUPtr m_indexBuffer;
        TextureUPtr m_texture;
        TextureUPtr m_texture2; // 2번째로 사용할 텍스쳐

        // 카메라 파라미터
        bool m_cameraControl { false };
        glm::vec2 m_prevMousePos { glm::vec2(0.0f) };

        float m_cameraPitch { 0.0f };
        float m_cameraYaw { 0.0f };

        glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) }; // 카메라 위치
        glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; // 카메라가 바라보는 타겟 (타겟 방향)
        glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) }; // 카매라 UP벡터

        // 종횡비 파라미터
        int m_width {WINDOW_WIDTH};
        int m_height {WINDOW_HEIGHT};

        // 빛 파라미터
        struct Light 
        {
            glm::vec3 position { glm::vec3(3.0f, 3.0f, 3.0f) };
            glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
            glm::vec3 diffuse { glm::vec3(0.5f, 0.5f, 0.5f) };
            glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
        };
        Light m_light;

        // 머티리얼 파라미터
        struct Material 
        {
            TextureUPtr diffuse;
            TextureUPtr specular;
            float shininess { 32.0f };
        };
        Material m_material;

        // 기타 디버깅용
        bool m_animation { true };
};

#endif // __CONTEXT_H__