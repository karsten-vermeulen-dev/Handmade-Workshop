#pragma once
#include <Windows.h>
#include <map>
#include <string>
#include <vector>
#include "GLAD/gl.h"
#include <glm.hpp>
#include <gtc/quaternion.hpp>

class Utility
{

public:

	enum class Severity
	{
		Failure = MB_ICONERROR,
		Warning = MB_ICONWARNING,
		Default = MB_ICONINFORMATION
	};

	enum class Destination
	{
		LogFile,
		WindowsMessageBox
	};

	static void SetWindowHandle(HWND windowHandle);

	static void CheckGLError();
	static void DisplayProfile();
	static void DisplayExtensions();

	static void RemoveCharacter(std::string& string, char character);

	static void ParseString(std::string& string,
		std::vector<std::string>& subStrings, char token);

	static bool LoadConfigFile(const std::string& filename,
		std::map<std::string, std::string>& dataMap);

	static bool LoadShaderFile(const std::string& filename,
		std::string& sourceCode);

	static std::string NumToStr(GLfloat value);
	static std::string NumToStr(const glm::vec2& value);
	static std::string NumToStr(const glm::vec3& value);
	static std::string NumToStr(const glm::vec4& value);
	static std::string NumToStr(const glm::quat& value);

	static void Log(Destination destination,
		GLfloat value, const std::string& label = "");
	static void Log(Destination destination,
		const glm::vec2& value, const std::string& label = "");
	static void Log(Destination destination,
		const glm::vec3& value, const std::string& label = "");
	static void Log(Destination destination,
		const glm::vec4& value, const std::string& label = "");
	static void Log(Destination destination,
		const glm::quat& value, const std::string& label = "");
	static void Log(Destination destination,
		const std::string& message, Severity severity = Severity::Default);
	static void Log(Destination destination,
		GLfloat x, GLfloat y, GLfloat z = 0.0f, GLfloat w = 1.0f, const std::string& label = "");

private:

	static HWND windowHandle;

};