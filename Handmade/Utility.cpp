#include <fstream>
#include "Utility.h"

HWND Utility::windowHandle = nullptr;
//======================================================================================================
void Utility::CheckGLError()
{
	//This will return the first error that 
	//occured since this function was last called
	GLenum errorCode = glGetError();

	switch (errorCode)
	{

	case GL_NO_ERROR:
	{
		Log(Destination::LogFile, "There are no errors.", Severity::Default);
		break;
	}

	case GL_INVALID_ENUM:
	{
		Log(Destination::LogFile, "Invalid enumeration type passed.", Severity::Failure);
		break;
	}

	case GL_INVALID_VALUE:
	{
		Log(Destination::LogFile, "Numeric value outside of accepted range.", Severity::Failure);
		break;
	}

	case GL_INVALID_OPERATION:
	{
		Log(Destination::LogFile, "Invalid operation.", Severity::Failure);
		break;
	}

	case GL_INVALID_FRAMEBUFFER_OPERATION:
	{
		Log(Destination::LogFile, "Framebuffer object incomplete.", Severity::Failure);
		break;
	}

	case GL_STACK_OVERFLOW:
	{
		Log(Destination::LogFile, "Stack overflow.", Severity::Failure);
		break;
	}

	case GL_STACK_UNDERFLOW:
	{
		Log(Destination::LogFile, "Stack underflow.", Severity::Failure);
		break;
	}

	case GL_OUT_OF_MEMORY:
	{
		Log(Destination::LogFile, "Out of memory.", Severity::Failure);
		break;
	}

	}
}
//======================================================================================================
void Utility::DisplayProfile()
{
	char text[50];
	strcpy_s(text, "Graphics card vendor: ");
	strcat_s(text, reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	Utility::Log(Destination::LogFile, text);

	strcpy_s(text, "Graphics card model: ");
	strcat_s(text, reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	Utility::Log(Destination::LogFile, text);

	strcpy_s(text, "OpenGL version: ");
	strcat_s(text, reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	Utility::Log(Destination::LogFile, text);

	strcpy_s(text, "GLSL version: ");
	strcat_s(text, reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
	Utility::Log(Destination::LogFile, text);
	Utility::Log(Destination::LogFile, "=================================================");
}
//======================================================================================================
void Utility::DisplayExtensions()
{
	GLint totalExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &totalExtensions);

	for (GLint i = 0; i < totalExtensions; i++)
	{
		char text[100];
		strcpy_s(text, (std::to_string(i + 1)).c_str());
		strcat_s(text, ". ");
		strcat_s(text, reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
		Utility::Log(Destination::LogFile, text);
	}

	Utility::Log(Destination::LogFile, "=================================================");
}
//======================================================================================================
void Utility::SetWindowHandle(HWND windowHandle)
{
	windowHandle = windowHandle;
}
//======================================================================================================
void Utility::RemoveCharacter(std::string& string, char character)
{
	auto it = std::find(string.begin(), string.end(), character);

	if (it != string.end())
	{
		do
		{
			string.erase(it);
			it = std::find(string.begin(), string.end(), character);
		} while (it != string.end());
	}
}
//======================================================================================================
void Utility::ParseString(std::string& string, std::vector<std::string>& subStrings, char token)
{
	size_t start = 0;
	size_t end = 0;

	assert(!string.empty());

	while (end != std::string::npos)
	{
		end = string.find(token, start);
		if ((end - start) > 0)
		{
			subStrings.push_back(string.substr(start, end - start));
		}
		start = end + 1;
	}
}
//======================================================================================================
bool Utility::LoadConfigFile(const std::string& filename, std::map<std::string, std::string>& dataMap)
{
	std::fstream file(filename, std::ios_base::in);

	if (!file.is_open())
	{
		return false;
	}

	std::string line;

	while (!file.eof())
	{
		std::getline(file, line);
		std::vector<std::string> subStrings;
		ParseString(line, subStrings, '=');

		if (!subStrings.empty())
		{
			dataMap[subStrings[0]] = subStrings[1];
		}
	}

	file.close();
	return true;
}
//======================================================================================================
bool Utility::LoadShaderFile(const std::string& filename, std::string& sourceCode)
{
	std::fstream file(filename, std::ios_base::in);

	if (!file.is_open())
	{
		return false;
	}

	std::string line;

	while (!file.eof())
	{
		getline(file, line);
		sourceCode += line + "\n";
	}

	file.close();
	return true;
}
//======================================================================================================
std::string Utility::NumToStr(GLfloat value)
{
	return std::to_string(value);
}
//======================================================================================================
std::string Utility::NumToStr(const glm::vec2& value)
{
	return std::to_string(value.x) + ", " + std::to_string(value.y);
}
//======================================================================================================
std::string Utility::NumToStr(const glm::vec3& value)
{
	return std::to_string(value.x) + ", "
		+ std::to_string(value.y) + ", "
		+ std::to_string(value.z);
}
//======================================================================================================
std::string Utility::NumToStr(const glm::vec4& value)
{
	return std::to_string(value.x)
		+ ", " + std::to_string(value.y)
		+ ", " + std::to_string(value.z)
		+ ", " + std::to_string(value.w);
}
//======================================================================================================
std::string Utility::NumToStr(const glm::quat& value)
{
	return std::to_string(value.x)
		+ ", " + std::to_string(value.y)
		+ ", " + std::to_string(value.z)
		+ ", " + std::to_string(value.w);
}
//======================================================================================================
void Utility::Log(Destination destination, const glm::vec2& value, const std::string& label)
{
	Log(destination, value.x, value.y, 0.0f, 0.0f, label);
}
//======================================================================================================
void Utility::Log(Destination destination, const glm::vec3& value, const std::string& label)
{
	Log(destination, value.x, value.y, value.z, 0.0f, label);
}
//======================================================================================================
void Utility::Log(Destination destination, const glm::vec4& value, const std::string& label)
{
	Log(destination, value.x, value.y, value.z, value.w, label);
}
//======================================================================================================
void Utility::Log(Destination destination, const glm::quat& value, const std::string& label)
{
	Log(destination, value.x, value.y, value.z, value.w, label);
}
//======================================================================================================
void Utility::Log(Destination destination, GLfloat value, const std::string& label)
{
	if (!label.empty())
	{
		if (destination == Destination::WindowsMessageBox)
		{
			MessageBox(windowHandle,
				reinterpret_cast<LPCWSTR>(std::to_wstring(value).c_str()),
				reinterpret_cast<LPCWSTR>(std::wstring(label.begin(), label.end()).c_str()),
				MB_ICONINFORMATION | MB_OK);
		}

		else if (destination == Destination::LogFile)
		{
			std::fstream file("Data/Output.log", std::ios_base::out | std::ios_base::app);
			std::string message = "[" + label + "] " + std::to_string(value) + "\n";
			file << message;
			file.close();
		}
	}
}
//======================================================================================================
void Utility::Log(Destination destination, const std::string& message, Severity severity)
{
	if (!message.empty())
	{
		if (destination == Destination::WindowsMessageBox)
		{
			MessageBox(windowHandle,
				reinterpret_cast<LPCWSTR>(std::wstring(message.begin(), message.end()).c_str()),
				L"Log", static_cast<GLuint>(severity) | MB_OK);
		}

		else if (destination == Destination::LogFile)
		{
			std::string finalMessage;
			std::fstream file("Data/Output.log", std::ios_base::out | std::ios_base::app);

			if (severity == Severity::Failure)
			{
				finalMessage = "[FAILURE] " + message + "\n";
			}

			else if (severity == Severity::Warning)
			{
				finalMessage = "[WARNING] " + message + "\n";
			}

			else
			{
				finalMessage = message + "\n";
			}

			file << finalMessage;
			file.close();
		}
	}
}
//======================================================================================================
void Utility::Log(Destination destination,
	GLfloat x, GLfloat y, GLfloat z, GLfloat w, const std::string& label)
{
	if (!label.empty())
	{
		if (destination == Destination::WindowsMessageBox)
		{
			std::string message = "x = " + std::to_string(x) +
				", y = " + std::to_string(y) +
				", z = " + std::to_string(z) +
				", w = " + std::to_string(w);

			MessageBox(windowHandle,
				reinterpret_cast<LPCWSTR>(std::wstring(message.begin(), message.end()).c_str()),
				reinterpret_cast<LPCWSTR>(std::wstring(label.begin(), label.end()).c_str()),
				MB_ICONINFORMATION | MB_OK);
		}

		else if (destination == Destination::LogFile)
		{
			std::fstream file("Data/Output.log", std::ios_base::out | std::ios_base::app);
			std::string message = "[" + label + "] " + "x = " + std::to_string(x) +
				", y = " + std::to_string(y) +
				", z = " + std::to_string(z) +
				", w = " + std::to_string(w) + "\n";
			file << message;
			file.close();
		}
	}
}