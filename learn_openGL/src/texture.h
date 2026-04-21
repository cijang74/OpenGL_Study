#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "image.h"

CLASS_PTR(Texture)
class Texture 
{
    public:
        // ImagePtr이나 ImageUPtr이 아닌 Image*를 인자로 사용하는 이유: 굳이 소유권 넘겨서 이것 저것 할 필요 없이, gpu 복사만 하고 안쓸거기떄문에 쌩 포인터 씀
        static TextureUPtr CreateFromImage(const Image* image);
        ~Texture();

        const uint32_t Get() const { return m_texture; }
        void Bind() const;
        void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
        void SetWrap(uint32_t sWrap, uint32_t tWrap) const;

    private:
        Texture() {}
        void CreateTexture();
        void SetTextureFromImage(const Image* image);

        uint32_t m_texture { 0 };
};

#endif // __TEXTURE_H__