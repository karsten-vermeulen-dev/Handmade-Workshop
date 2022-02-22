#include <gtc\matrix_transform.hpp>
#include "Transform.h"

//======================================================================================================
Transform::Transform(const glm::mat4& matrix)
		  :m_matrix(matrix){}
//======================================================================================================
glm::vec3 Transform::GetEulerAngles()
{
	return glm::degrees(glm::eulerAngles(m_rotation));
}
//======================================================================================================
const glm::vec3& Transform::GetPosition() const
{
	return m_position;
}
//======================================================================================================
const glm::quat& Transform::GetRotation() const
{
	return m_rotation;
}
//======================================================================================================
const glm::vec3& Transform::GetScale() const
{
	return m_scale;
}
//======================================================================================================
const glm::mat4& Transform::GetMatrix()
{
	Update();
	return m_matrix;
}
//======================================================================================================
void Transform::SetIdentity()
{
	m_isDirty = false;
	m_scale = glm::vec3(1.0f);
	m_matrix = glm::mat4(1.0f);
	m_position = glm::vec3(0.0f);
	m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}
//======================================================================================================
void Transform::SetPosition(const glm::vec3& position)
{
	m_position = position;
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(const glm::quat& rotation)
{
	m_rotation = rotation;
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(const glm::vec3& eulerAngles)
{
	m_rotation = glm::quat(glm::radians(eulerAngles));
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(GLfloat angle, const glm::vec3& axis)
{
	m_rotation = glm::quat(glm::radians(angle), axis);
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetRotation(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	m_rotation = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetScale(GLfloat scale)
{
	m_scale.x = scale;
	m_scale.y = scale;
	m_scale.z = scale;
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetScale(const glm::vec3& scale)
{
	m_scale = scale;
	m_isDirty = true;
}
//======================================================================================================
void Transform::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
	m_isDirty = true;
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
		m_position += glm::vec3(m_matrix * glm::vec4(x, y, z, 0.0f));
	}

	else
	{
		m_position += glm::vec3(x, y, z);
	}

	m_isDirty = true;
}
//======================================================================================================
void Transform::Rotate(const glm::quat& rotation, Space space)
{
	glm::quat tempRotation = rotation;

	if (space == Space::Local)
	{
		m_rotation = m_rotation * tempRotation;
	}

	else
	{
		m_rotation = tempRotation * m_rotation;
	}

	m_isDirty = true;
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
	m_scale *= glm::vec3(scale, scale, scale);
	m_isDirty = true;
}
//======================================================================================================
void Transform::Scale(const glm::vec3& scale)
{
	m_scale *= scale;
	m_isDirty = true;
}
//======================================================================================================
void Transform::Scale(GLfloat x, GLfloat y, GLfloat z)
{
	m_scale *= glm::vec3(x, y, z);
	m_isDirty = true;
}
//======================================================================================================
void Transform::Update()
{
	if (m_isDirty)
	{
		m_matrix = glm::translate(glm::mat4(1.0f), m_position);
		m_matrix *= glm::mat4_cast(m_rotation);
		m_matrix = glm::scale(m_matrix, m_scale);
		m_isDirty = false;
	}
}