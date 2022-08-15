#pragma once
#include <map>
#include <string>
#include "GLAD/gl.h"
#include <glm.hpp>

class Shader
{

public:

	static bool Initialize();
	static void Shutdown();

	GLuint GetUniformID(const std::string& uniform) const;
	GLuint GetAttributeID(const std::string& attribute) const;

	bool Create(const std::string& vertexShaderFilename,
		const std::string& fragmentShaderFilename);

	void BindUniform(const std::string& uniform);
	void BindAttribute(const std::string& attribute);

	void SendData(const std::string& uniform, GLint intData) const;
	void SendData(const std::string& uniform, GLuint uintData) const;
	void SendData(const std::string& uniform, GLfloat floatData) const;
	void SendData(const std::string& uniform, const glm::vec2& vec2Data) const;
	void SendData(const std::string& uniform, const glm::vec3& vec3Data) const;
	void SendData(const std::string& uniform, const glm::vec4& vec4Data) const;

	void SendData(const std::string& uniform,
		const glm::mat3& matrix3x3, bool transposed = false) const;

	void SendData(const std::string& uniform,
		const glm::mat4& matrix4x4, bool transposed = false) const;

	void Use() const;
	void Destroy() const;

private:

	bool LinkProgram();
	bool CompileShaders(const std::string& filename);

	static GLint vertexShaderID;
	static GLint fragmentShaderID;

	GLuint shaderProgramID{ 0 };
	std::map<std::string, GLuint> uniforms;
	std::map<std::string, GLuint> attributes;

};