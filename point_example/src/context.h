#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

// 프로그램 라이프사이클을 고려하여 코드 리팩토링
// OpenGL object 들을 관리하고 렌더링하는 코드를 분리하고자 context.h, context.cpp 작성

CLASS_PTR(Context)
class Context 
{
    public:
        static ContextUPtr Create();
        void Render();

    private:
        Context() {}
        bool Init();
        ProgramUPtr m_program;

        // uint32_t m_vertexArrayObject; // VAO 변수 선언
        // uint32_t m_vertexBuffer; // VBO(정점 데이터의 정보)를 담아둘 변수 선언
        // uint32_t m_indexBuffer; // EBO 변수 선언

        VertexLayoutUPtr m_vertexLayout;
        BufferUPtr m_vertexBuffer;
        BufferUPtr m_indexBuffer;
};

#endif // __CONTEXT_H__