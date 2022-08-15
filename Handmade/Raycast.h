#pragma once
#include "GLAD/gl.h"
#include <glm.hpp>

class Raycast
{
public:

	const glm::vec3& GetOrigin() const;
	const glm::vec3& GetDirection() const;

	void SetOrigin(const glm::vec3& origin);
	void SetOrigin(GLfloat x, GLfloat y, GLfloat z);

	void SetDirection(const glm::vec3& direction);
	void SetDirection(GLfloat x, GLfloat y, GLfloat z);

private:

	glm::vec3 origin{ 0.0f };
	glm::vec3 direction{ 0.0f };

};