#include "vertex_layout.h"

VertexLayoutUPtr VertexLayout::Create() 
{
    auto vertexLayout = VertexLayoutUPtr(new VertexLayout());
    vertexLayout->Init();
    return std::move(vertexLayout);
}

VertexLayout::~VertexLayout() 
{
    if (m_vertexArrayObject) 
    {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }
}

void VertexLayout::Bind() const 
{
    // 지금부터 사용할 VAO 설정
    glBindVertexArray(m_vertexArrayObject);
}

void VertexLayout::SetAttrib(uint32_t attribIndex, int count, uint32_t type, bool normalized, size_t stride, uint64_t offset) const 
{
    glEnableVertexAttribArray(attribIndex); // 정점 attribute 중 0번째를 사용하도록 설정

    // 정점의 0번째 attribute, 해당 attribute는 3개의 값으로 구성(x, y, z), attribute의 데이터 타입은 float, 0~1로 정규화 되어있는가? <- FALSE,
    // 두 정점의 간격은  sizeof(float) * 3 -> 12byte이고, 첫 정점의 해당 attribute까지의 간격은 0이다.
    glVertexAttribPointer(attribIndex, count, type, normalized, stride, (const void*)offset);
}

void VertexLayout::Init() 
{
    // 1개의 VAO 생성하여 m_vertexArrayObject에 할당
    glGenVertexArrays(1, &m_vertexArrayObject);
    Bind();
}