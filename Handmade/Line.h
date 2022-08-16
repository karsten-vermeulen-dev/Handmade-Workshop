#pragma once
#include "GLAD/gl.h"
#include <glm.hpp>
#include "Buffer.h"
#include "Object.h"

//TODO - Consider not deriving this from Object
//TODO - Add line start/end position to ctor so that we have an actual line to begin with
//TODO - Add SetStart/end position functions to set the absolute values of the line segment

class Line : public Object
{

public:

	Line(const std::string& tag,
		GLfloat lineWidth = 2.0f, 
		GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f);
	virtual ~Line();

	void SetEndPoints(GLfloat x1, GLfloat y1, GLfloat z1,
		GLfloat x2, GLfloat y2, GLfloat z2);

	virtual void SetColor(const glm::vec4& color);
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	Buffer buffer;
	GLfloat lineWidth;

};