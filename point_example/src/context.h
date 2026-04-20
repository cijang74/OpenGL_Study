#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"

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
};

#endif // __CONTEXT_H__