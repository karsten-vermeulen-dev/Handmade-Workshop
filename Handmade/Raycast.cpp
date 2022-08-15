#include "Raycast.h"

//======================================================================================================
const glm::vec3& Raycast::GetOrigin() const
{
	return origin;
}
//======================================================================================================
const glm::vec3& Raycast::GetDirection() const
{
	return direction;
}
//======================================================================================================
void Raycast::SetOrigin(const glm::vec3& origin)
{
	this->origin = origin;
}
//======================================================================================================
void Raycast::SetOrigin(GLfloat x, GLfloat y, GLfloat z)
{
	origin.x = x;
	origin.y = y;
	origin.z = z;
}
//======================================================================================================
void Raycast::SetDirection(const glm::vec3& direction)
{
	this->direction = direction;
}
//======================================================================================================
void Raycast::SetDirection(GLfloat x, GLfloat y, GLfloat z)
{
	direction.x = x;
	direction.y = y;
	direction.z = z;
}