#pragma once
#include <array>
#include "GLAD/gl.h"
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include "BoxCollider.h"
#include "SphereCollider.h"

class OBBCollider
{

public:

	const glm::vec3& GetScale() const;
	const glm::vec3& GetPosition() const;
	const glm::quat& GetRotation() const;
	const glm::vec3& GetDimension() const;

	void SetScale(const glm::vec3& scale);
	void SetScale(GLfloat x, GLfloat y, GLfloat z);

	void SetPosition(const glm::vec3& position);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);

	void SetRotation(const glm::quat& rotation);
	void SetRotation(const glm::vec3& eulerAngles);
	void SetRotation(GLfloat pitch, GLfloat yaw, GLfloat roll);

	void SetDimension(const glm::vec3& dimension);
	void SetDimension(GLfloat width, GLfloat height, GLfloat depth);

	bool IsColliding(const OBBCollider& secondBox) const;
	bool IsColliding(const BoxCollider& secondBox) const;
	bool IsColliding(const SphereCollider& secondSphere) const;

	glm::vec3 PointOnBox(const glm::vec3& point) const;
	glm::vec3 PointOnBox(GLfloat x, GLfloat y, GLfloat z) const;

	void Update();
	void Render() { /*debug only*/ }

private:

	glm::vec3 upAxis{ 0.0f, 1.0f, 0.0f };
	glm::vec3 rightAxis{ 1.0f, 0.0f, 0.0f };
	glm::vec3 forwardAxis{ 0.0f, 0.0f, -1.0f };
	std::array<glm::vec3, 8> corners{ glm::vec3(0.0f) };

	glm::vec3 scale{ 1.0f };
	glm::vec3 position{ 0.0f };
	glm::vec3 dimension{ 1.0f };
	glm::vec3 halfDimension{ 0.5f };
	glm::vec4 color{ 1.0f, 0.5f, 0.0f, 0.4f };
	glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };

};