#include <assert.h>
#include <fstream>
#include "Shader.h"
#include "Utility.h"

GLint Shader::vertexShaderID = 0;
GLint Shader::fragmentShaderID = 0;

//======================================================================================================
bool Shader::Initialize()
{
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	if (vertexShaderID == 0)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error creating vertex shader object. Possible causes could be a "
			"very old graphics card that does not support modern OpenGL.",
			Utility::Severity::Failure);
		return false;
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	if (fragmentShaderID == 0)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error creating fragment shader object. Possible causes could be a "
			"very old graphics card that does not support modern OpenGL.",
			Utility::Severity::Failure);
		return false;
	}

	return true;
}
//======================================================================================================
void Shader::Shutdown()
{
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}
//======================================================================================================
GLuint Shader::GetUniformID(const std::string& uniform) const
{
	auto it = uniforms.find(uniform);
	assert(it != uniforms.end());
	return it->second;
}
//======================================================================================================
GLuint Shader::GetAttributeID(const std::string& attribute) const
{
	auto it = attributes.find(attribute);
	assert(it != attributes.end());
	return it->second;
}
//======================================================================================================
bool Shader::Create(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	shaderProgramID = glCreateProgram();

	//If the shader program ID is 0 it means an error occurred. Possible causes are 
	//that GLAD has not been set up properly yet or your graphics card is very old
	if (shaderProgramID == 0)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error creating shader program. Possible causes could be a "
			"very old graphics card that does not support modern OpenGL.",
			Utility::Severity::Failure);
		return false;
	}

	if (!CompileShaders(vertexShaderFilename))
	{
		return false;
	}

	if (!CompileShaders(fragmentShaderFilename))
	{
		return false;
	}

	if (!LinkProgram())
	{
		return false;
	}

	return true;
}
//======================================================================================================
void Shader::BindUniform(const std::string& uniform)
{
	auto it = uniforms.find(uniform);

	if (it == uniforms.end())
	{
		//Unbound shader uniforms are either 
		//not present in the shader or unused
		auto ID = glGetUniformLocation(shaderProgramID, uniform.c_str());
		assert(ID != -1);
		uniforms[uniform] = ID;
	}
}
//======================================================================================================
void Shader::BindAttribute(const std::string& attribute)
{
	auto it = attributes.find(attribute);

	if (it == attributes.end())
	{
		//Unbound shader attributes are either 
		//not present in the shader or unused
		auto ID = glGetAttribLocation(shaderProgramID, attribute.c_str());
		assert(ID != -1);
		attributes[attribute] = ID;
	}
}
//======================================================================================================
void Shader::SendData(const std::string& uniform, GLint intData) const
{
	glUniform1i(GetUniformID(uniform), intData);
}
//======================================================================================================
void Shader::SendData(const std::string& uniform, GLuint uintData) const
{
	glUniform1ui(GetUniformID(uniform), uintData);
}
//======================================================================================================
void Shader::SendData(const std::string& uniform, GLfloat floatData) const
{
	glUniform1f(GetUniformID(uniform), floatData);
}
//======================================================================================================
void Shader::SendData(const std::string& uniform, const glm::vec2& vec2Data) const
{
	glUniform2fv(GetUniformID(uniform), 1, &vec2Data.x);
}
//======================================================================================================
void Shader::SendData(const std::string& uniform, const glm::vec3& vec3Data) const
{
	glUniform3fv(GetUniformID(uniform), 1, &vec3Data.x);
}
//======================================================================================================
void Shader::SendData(const std::string& uniform, const glm::vec4& vec4Data) const
{
	glUniform4fv(GetUniformID(uniform), 1, &vec4Data.x);
}
//======================================================================================================
void Shader::SendData(const std::string& uniform,
	const glm::mat3& matrix3x3, bool transposed) const
{
	glUniformMatrix3fv(GetUniformID(uniform), 1, transposed, &matrix3x3[0][0]);
}
//======================================================================================================
void Shader::SendData(const std::string& uniform,
	const glm::mat4& matrix4x4, bool transposed) const
{
	glUniformMatrix4fv(GetUniformID(uniform), 1, transposed, &matrix4x4[0][0]);
}
//======================================================================================================
void Shader::Use() const
{
	glUseProgram(shaderProgramID);
}
//======================================================================================================
void Shader::Destroy() const
{
	glDeleteProgram(shaderProgramID);
}
//======================================================================================================
bool Shader::LinkProgram()
{
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);

	GLint result = 0;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		GLchar error[1000];
		GLsizei bufferSize = 1000;
		glGetProgramInfoLog(shaderProgramID, bufferSize, &bufferSize, error);
		Utility::Log(Utility::Destination::WindowsMessageBox, error, Utility::Severity::Failure);
		return false;
	}

	return true;
}
//======================================================================================================
bool Shader::CompileShaders(const std::string& filename)
{
	std::string sourceCode;

	if (!Utility::LoadShaderFile(filename, sourceCode))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error loading shader file \"" + (filename)+"\"."
			"Possible causes could be a corrupt or missing file. It could also be "
			"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
		return false;
	}

	std::vector<std::string> subStrings;
	Utility::ParseString(const_cast<std::string&>(filename), subStrings, '.');
	auto shaderID = (subStrings[1] == "vert") ? vertexShaderID : fragmentShaderID;

	const GLchar* finalCode = reinterpret_cast<const GLchar*>(sourceCode.c_str());
	glShaderSource(shaderID, 1, &finalCode, nullptr);
	glCompileShader(shaderID);

	GLint compileResult;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == GL_FALSE)
	{
		GLchar error[1000];
		GLsizei bufferSize = 1000;
		glGetShaderInfoLog(shaderID, bufferSize, &bufferSize, error);
		Utility::Log(Utility::Destination::WindowsMessageBox, error, Utility::Severity::Failure);
		return false;
	}

	return true;
}