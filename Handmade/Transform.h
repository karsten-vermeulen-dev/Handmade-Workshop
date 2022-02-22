#pragma once

/*===================================================================#
| 'Transform' source files last updated on 23 November 2021          |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

#include "GLAD/gl.h"
#include <glm.hpp>
#include <gtc/quaternion.hpp>

class Transform
{
public:
	enum class Space
	{
		Local,
		Global
	};
	Transform(const glm::mat4& matrix = glm::mat4(1.0f));

	glm::vec3 GetEulerAngles();
	
	const glm::vec3& GetPosition() const;
	const glm::quat& GetRotation() const;
	const glm::vec3& GetScale() const;
	const glm::mat4& GetMatrix();
	
	void Rotate(const glm::quat& rotation, Space space = Space::Global);
	void Rotate(const glm::vec3& eulerAngles, Space space = Space::Global);
	void Rotate(GLfloat angle, const glm::vec3& axis, Space space = Space::Global);
	void Rotate(GLfloat pitch, GLfloat yaw, GLfloat roll, Space space = Space::Global);
	
	void Scale(const glm::vec3& scale);
	void Scale(GLfloat scale);
	void Scale(GLfloat x, GLfloat y, GLfloat z);
	
	void SetIdentity();
	
	void SetPosition(const glm::vec3& position);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	
	void SetRotation(const glm::quat& rotation);
	void SetRotation(const glm::vec3& eulerAngles);
	void SetRotation(GLfloat angle, const glm::vec3& axis);
	void SetRotation(GLfloat pitch, GLfloat yaw, GLfloat roll);
	
	void SetScale(const glm::vec3& scale);
	void SetScale(GLfloat scale);
	void SetScale(GLfloat x, GLfloat y, GLfloat z);
	
	void Translate(const glm::vec3& translation, Space space = Space::Global);
	void Translate(GLfloat x, GLfloat y, GLfloat z, Space space = Space::Global);

	void Update();
private:
	bool m_isDirty = false;

	glm::mat4 m_matrix = glm::mat4{0};
	glm::quat m_rotation = glm::quat(1.0f,0.0f,0.0f,0.0f);
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
};