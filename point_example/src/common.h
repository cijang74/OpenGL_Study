// 프로젝트가 공통적으로 이용할 기능들을 정의할 파일
#ifndef __COMMON_H__ // __COMMON_H__이 정의되지 않았다면 3~12라인을 실행한다.
#define __COMMON_H__

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h> // 주의: 반드시 #include <GLFW/glfw3.h> 이전에 추가할 것. 안그러면 오류 생김
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

// std::unique_ptr 대신 클래스이름UPtr 사용할 수 있도록 매크로 추가
// klassName ## UPtr는 내부에서 klassNameUPtr으로 해석됨. klassNameUPtr이라고 프로그래머가 사용하면 컴파일러에서는 std::unique_ptr<klassName>로 자동 해석
#define CLASS_PTR(klassName) \
class klassName; \
using klassName ## UPtr = std::unique_ptr<klassName>; \
using klassName ## Ptr = std::shared_ptr<klassName>; \
using klassName ## WPtr = std::weak_ptr<klassName>;

// optional: 어떤 값이 있거나 없는 경우를 포인터 없이 표현 가능
std::optional<std::string> LoadTextFile(const std::string& filename);

#endif // __COMMON_H__