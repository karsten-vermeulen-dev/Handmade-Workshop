#pragma once
#include "Buffer.h"
#include "Object.h"

class Cuboid : public Object
{

public:

	static int GetTotalCuboids();

	Cuboid(const std::string& tag, GLfloat width = 1.0f, GLfloat height = 1.0f, GLfloat depth = 1.0f,
		GLfloat r = 0.5f, GLfloat g = 0.5f, GLfloat b = 0.5f, GLfloat a = 1.0f);
	virtual ~Cuboid();

	void SetTextureScale(GLfloat width, GLfloat height);

	void SetDimension(const glm::vec3& dimension);
	void SetDimension(GLfloat width, GLfloat height, GLfloat depth);

	virtual void SetColor(const glm::vec4& color);
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	static int totalCuboids;

	Buffer buffer;
	glm::vec3 dimension;

};