#include "buffer.h"

// 버퍼의 용도, GPU 버퍼에서 어떻게 사용할 것인지, 데이터 포인터, 데이터 길이를 입력받아 새로운 버퍼 생성
BufferUPtr Buffer::CreateWithData(uint32_t bufferType, uint32_t usage, const void* data, size_t dataSize) 
{
    auto buffer = BufferUPtr(new Buffer());
    if (!buffer->Init(bufferType, usage, data, dataSize))
    {
        return nullptr;
    }
    return std::move(buffer);
}

Buffer::~Buffer() 
{
    if (m_buffer) 
    {
        glDeleteBuffers(1, &m_buffer);
    }
}

void Buffer::Bind() const 
{
    glBindBuffer(m_bufferType, m_buffer); // // 지금부터 사용할 buffer object를 지정 -> m_buffer는 m_bufferType 용도로 쓸거야.
}

bool Buffer::Init(uint32_t bufferType, uint32_t usage, const void* data, size_t dataSize) 
{
    m_bufferType = bufferType;
    m_usage = usage;
    glGenBuffers(1, &m_buffer);// 새로운 buffer object를 만들어서 m_buffer 할당
    Bind();
    glBufferData(m_bufferType, dataSize, data, usage);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
    // // -> GL_ARRAY_BUFFER의 형태고(버퍼의 용도), vertices에는 총 9개의 점이 있음(크기). 정점 데이터 구조는 vertices고(포인터),
    // // 맨 끝의 인자값을 통해 gpu 버퍼에서 어떻게 사용할지를 정해줄 수 있음. GL_STATIC_DRAW는 이번 세팅 이후 다시는 바꾸지 않을 것을 의미)
   return true;
}