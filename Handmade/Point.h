#pragma once
#include "GLAD/gl.h"
#include <glm.hpp>
#include "Buffer.h"
#include "Object.h"

//TODO - Consider not deriving this from Object
//TODO - Add position value to the ctor (use David's MFC app as a reference)

class Point : public Object
{

public:

	Point(const std::string& tag,
		GLfloat pointSize = 5.0f,
		GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f, GLfloat a = 1.0f);
	virtual ~Point();

	virtual void SetColor(const glm::vec4& color);
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	Buffer buffer;
	GLfloat pointSize;

};