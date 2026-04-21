#include "texture.h"

TextureUPtr Texture::CreateFromImage(const Image* image) 
{
    // 공간 할당
    auto texture = TextureUPtr(new Texture());

    // 텍스쳐 변수 생성
    texture->CreateTexture();
    texture->SetTextureFromImage(image);
    return std::move(texture);
}

Texture::~Texture() 
{
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }
}

void Texture::Bind() const 
{
    // GL_TEXTURE_2D 형태의 모든 텍스쳐가 m_texture를 사용하도록 지정
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const 
{
    // glTexParameteri: Texture Filter와 Texture Warpping과 관련된 옵션 파라미터 세팅(int값)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // MIN_FILTER: 이미지가 많이 축소되었을 때 사용하는 필터 -> 리니어 필터 사용
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // MAG_FILTER: 이미지가 많이 확대되었을 떄 사용하는 필터 -> 리니어 필터 사용
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const 
{
    // glTexParameteri: Texture Filter와 Texture Warpping과 관련된 옵션 파라미터 세팅(int값)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // s는 가로축(x축)을 의미, x축이 -1~1을 벗어나면 GL_CLAMP_TO_EDGE로 처리
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // t는 세로축(y축)을 의미, y축이 -1~1을 벗어나면 GL_CLAMP_TO_EDGE로 처리
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}
    
void Texture::CreateTexture() 
{
    // 깡 텍스쳐 객체 1개 생성하여 변수에 id 할당
    glGenTextures(1, &m_texture);

    Bind();
    SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Texture::SetTextureFromImage(const Image* image) 
{
    GLenum format = GL_RGBA;
    switch (image->GetChannelCount()) 
    {
        default: break;
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
    }
    
    // CPU에서 만들어놓은 image변수에 담긴 데이터를 GPU로 복사
    // GL_TEXTURE_2D: 이미지를 GPU로 복사할건데, 타겟은 2D Texture임(현재 m_texture 바인딩됨),
    // 민맵의 레벨값은 0(기본 이미지 사이즈)이고, GPU쪽에서 GL_RGB데이터 형태로 쓸거고, 이미지 크기는 다음과 같고, 
    // 이미지의 외각선(border)의 크기값은 0이고, 우리가 넘겨주는 이미지 픽셀 타입은 GL_RGB데이터 형태이고,
    // 채널을 표현하는 데 쓰는 데이터 타입은 GL_UNSIGNED_BYTE 형태이고, 우리가 쓸 이미지의 포인터는 다음과 같다.
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetData());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0, format, GL_UNSIGNED_BYTE, image->GetData());

    // Minmap 데이터 준비
    glGenerateMipmap(GL_TEXTURE_2D);
}