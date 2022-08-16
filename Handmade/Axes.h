#pragma once
#include "Buffer.h"
#include "Model.h"
#include "Object.h"
#include "Shader.h"
#include "Tile.h"

class Axes : public Object
{

public:

	Axes(const std::string& tag, const std::string& filenameModel);
	Axes(const std::string& tag, GLint size = 15, GLfloat lineWidth = 5.0f);
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

	GLint size{ 0 };
	GLfloat lineWidth{ 0.0f };
	bool isPrimitive{ false };

	glm::vec3 arrowTipPositionX{ 0.0f };
	glm::vec3 arrowTipPositionY{ 0.0f };
	glm::vec3 arrowTipPositionZ{ 0.0f };

	Model model;
	Buffer buffer;

	static GLuint totalObjects;

};