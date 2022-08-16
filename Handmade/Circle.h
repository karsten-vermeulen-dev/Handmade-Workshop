#pragma once
#include "Buffer.h"
#include "Object.h"

class Circle : public Object
{

public:

	static int GetTotalCircles();

	Circle(const std::string& tag, GLfloat radius = 1.0f, GLuint slices = 25,
		GLfloat r = 0.5f, GLfloat g = 0.5f, GLfloat b = 0.5f, GLfloat a = 1.0f);
	virtual ~Circle();

	void SetRadius(GLfloat radius);
	
	virtual void SetColor(const glm::vec4& color);
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	static int totalCircles;

	GLuint slices;
	GLfloat radius;
	Buffer buffer;

};