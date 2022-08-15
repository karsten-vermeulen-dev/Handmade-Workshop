#include <gtc\matrix_transform.hpp>
#include "Transform.h"

//======================================================================================================
Transform::Transform(const glm::mat4& matrix) : matrix(matrix) {}
//======================================================================================================
glm::vec3 Transform::GetEulerAngles()
{
	return glm::degrees(glm::eulerAngles(rotation));
}
//======================================================================================================
const glm::vec3& Transform::GetPosition() const
{
	return position;
}
//======================================================================================================
const glm::quat& Transform::GetRotation() const
{
	return rotation;
}
//======================================================================================================
const glm::vec3& Transform::GetScale() const
{
	return scale;
}
//======================================================================================================
const glm::mat4& Transform::GetMatrix()
{
	Update();
	return matrix;
}
//======================================================================================================
void Transform::SetIdentity()
{
	isDirty = false;
	scale = glm::vec3(1.0f);
	matrix = glm::mat4(1.0f);
	position = glm::vec3(0.0f);
	rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}
//======================================================================================================
void Transform::SetPosition(const glm::vec3& position)
{
	this->position = position;
	isDirty = true;
}
//======================================================================================================
void Transform::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(const glm::quat& rotation)
{
	this->rotation = rotation;
	isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(const glm::vec3& eulerAngles)
{
	rotation = glm::quat(glm::radians(eulerAngles));
	isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(GLfloat angle, const glm::vec3& axis)
{
	rotation = glm::quat(glm::radians(angle), axis);
	isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	rotation = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
	isDirty = true;
}
//======================================================================================================
void Transform::SetScale(GLfloat scale)
{
	this->scale.x = scale;
	this->scale.y = scale;
	this->scale.z = scale;
	isDirty = true;
}
//======================================================================================================
void Transform::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
	isDirty = true;
}
//======================================================================================================
void Transform::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	isDirty = true;
}
//======================================================================================================
void Transform::Translate(const glm::vec3& translation, Space space)
{
	Translate(translation.x, translation.y, translation.z, space);
}
//======================================================================================================
void Transform::Translate(GLfloat x, GLfloat y, GLfloat z, Space space)
{
	//If we want to move in local space then we have to take rotation
	//into account so we multiply the current transformation matrix 
	//by the displacement vector made up of x, y and z.
	//We use a 0 instead of a 1 when multiplying because if we use a 
	//1 the vector would be treated as a vertex point and not a direction

	if (space == Space::Local)
	{
		position += glm::vec3(matrix * glm::vec4(x, y, z, 0.0f));
	}

	else
	{
		position += glm::vec3(x, y, z);
	}

	isDirty = true;
}
//======================================================================================================
void Transform::Rotate(const glm::quat& rotation, Space space)
{
	glm::quat tempRotation = rotation;

	if (space == Space::Local)
	{
		this->rotation = this->rotation * tempRotation;
	}

	else
	{
		this->rotation = tempRotation * this->rotation;
	}

	isDirty = true;
}
//======================================================================================================
void Transform::Rotate(const glm::vec3& eulerAngles, Space space)
{
	glm::quat tempRotation = glm::quat(glm::radians(eulerAngles));
	Rotate(tempRotation, space);
}
//======================================================================================================
//TODO - Fix this version of quaternion rotation as there seems to be an issue
void Transform::Rotate(GLfloat angle, const glm::vec3& axis, Space space)
{
	glm::quat tempRotation = glm::quat(glm::radians(angle), axis);
	Rotate(tempRotation, space);
}
//======================================================================================================
void Transform::Rotate(GLfloat pitch, GLfloat yaw, GLfloat roll, Space space)
{
	glm::quat tempRotation = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
	Rotate(tempRotation, space);
}
//======================================================================================================
void Transform::Scale(GLfloat scale)
{
	this->scale *= glm::vec3(scale, scale, scale);
	isDirty = true;
}
//======================================================================================================
void Transform::Scale(const glm::vec3& scale)
{
	this->scale *= scale;
	isDirty = true;
}
//======================================================================================================
void Transform::Scale(GLfloat x, GLfloat y, GLfloat z)
{
	scale *= glm::vec3(x, y, z);
	isDirty = true;
}
//======================================================================================================
void Transform::Update()
{
	if (isDirty)
	{
		matrix = glm::translate(glm::mat4(1.0f), position);
		matrix *= glm::mat4_cast(rotation);
		matrix = glm::scale(matrix, scale);
		isDirty = false;
	}
}