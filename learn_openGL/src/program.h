#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

CLASS_PTR(Program)
class Program 
{
    public:
        // vertex, fragment shader 외에 여러 개의 Shader를 링크할 수도 있게 하기 위해 vector타입을 매개변수로 받음
        static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders); // Shader 인스턴스는 다른 Program 인스턴스를 만드는 데 재사용할 수도 있음 -> ShaderPtr

        static ProgramUPtr Create(const std::string& vertShaderFilename, const std::string& fragShaderFilename);

        ~Program();
        uint32_t Get() const { return m_program; }  
        void Use() const;  

        void SetUniform(const std::string& name, int value) const;
        void SetUniform(const std::string& name, const glm::mat4& value) const;

        void SetUniform(const std::string& name, float value) const;
        void SetUniform(const std::string& name, const glm::vec3& value) const;

        void SetUniform(const std::string& name, const glm::vec4& value) const;

    private:
        Program() {}
        bool Link(const std::vector<ShaderPtr>& shaders);
        uint32_t m_program { 0 };
};

#endif // __PROGRAM_H__