#pragma once

/*===================================================================#
| 'cuboid' source files last updated on 28 September 2021            |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

#include "Buffer.h"
#include "Object.h"

class Cuboid : public Object
{

public:

	Cuboid(Object* parent,GLfloat width = 1.0f, GLfloat height = 1.0f, GLfloat depth = 1.0f,
		GLfloat r = 0.5f, GLfloat g = 0.5f, GLfloat b = 0.5f, GLfloat a = 1.0f);
	virtual ~Cuboid();

	const glm::vec4& GetColor() const;
	void SetTextureScale(GLfloat width, GLfloat height);

	void SetColor(const glm::vec4& color);
	void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	void SetDimension(const glm::vec3& dimension);
	void SetDimension(GLfloat width, GLfloat height, GLfloat depth);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	Buffer m_buffer;
	glm::vec3 m_dimension;
	bool m_linkOnce;

};