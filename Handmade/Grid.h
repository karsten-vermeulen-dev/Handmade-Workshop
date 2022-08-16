#pragma once
#include "Buffer.h"
#include "Object.h"

class Grid : public Object
{

public:

	Grid(const std::string& tag, bool hasSpaceForAxes = false,
		GLint size = 15, GLfloat lineWidth = 1.5f);
	virtual ~Grid();

	void SetSize(GLint size);
	void SetLineWidth(GLfloat lineWidth);

	virtual void SetColor(const glm::vec4& color);
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	void Create();

	Buffer buffer;
	GLint size{ 0 };
	GLfloat lineWidth{ 0.0f };
	bool hasSpaceForAxes{ false };

	const GLuint quadrants{ 4 };
};