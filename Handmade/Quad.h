#pragma once

/*===================================================================#
| 'Quad' source files last updated on 28 September 2021              |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

#include "Buffer.h"
#include "Object.h"

class Quad : public Object
{

public:

	Quad(GLfloat width = 1.0f, GLfloat height = 1.0f,
		GLfloat r = 0.5f, GLfloat g = 0.5f, GLfloat b = 0.5f, GLfloat a = 1.0f);
	virtual ~Quad();

	void SetTextureScale(GLfloat width, GLfloat height);

	void SetColor(const glm::vec4& color);
	void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	void SetDimension(const glm::vec2& dimension);
	void SetDimension(GLfloat width, GLfloat height);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	Buffer m_buffer;
	glm::vec2 m_dimension;
	bool m_linkOnce;
};