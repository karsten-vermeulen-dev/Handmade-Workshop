#pragma once
#include "Buffer.h"
#include "Object.h"

class Quad : public Object
{

public:

	static int GetTotalQuads();

	Quad(const std::string& tag, GLfloat width = 1.0f, GLfloat height = 1.0f,
		GLfloat r = 0.5f, GLfloat g = 0.5f, GLfloat b = 0.5f, GLfloat a = 1.0f);
	virtual ~Quad();

	void SetTextureScale(GLfloat width, GLfloat height);

	void SetDimension(const glm::vec2& dimension);
	void SetDimension(GLfloat width, GLfloat height);

	virtual void SetColor(const glm::vec4& color);
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	static int totalQuads;
	
	Buffer buffer;
	glm::vec2 dimension;

};