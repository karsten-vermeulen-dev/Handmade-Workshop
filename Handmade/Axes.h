#pragma once

/*===================================================================#
| 'Axes' source files last updated on 25 October 2021                |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

#include <algorithm>
#include "Buffer.h"
#include "Model.h"
#include "Object.h"
#include "Shader.h"
#include "Tile.h"

class Axes : public Object
{
public:

	Axes(const std::string& filenameModel);
	Axes(GLint size = 15, GLfloat lineWidth = 5.0f);
	virtual ~Axes();

	void SetSize(GLint size);
	void SetLineWidth(GLfloat lineWidth);

	const glm::vec3& GetArrowTipPositionX() const;
	const glm::vec3& GetArrowTipPositionY() const;
	const glm::vec3& GetArrowTipPositionZ() const;

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:
	void Create();

	GLint m_size;
	bool m_isPrimitive;
	GLfloat m_lineWidth;

	Model m_model;
	Buffer m_buffer;

	glm::vec3 m_arrowTipPositionX;
	glm::vec3 m_arrowTipPositionY;
	glm::vec3 m_arrowTipPositionZ;

	static GLuint s_totalObjects;
};