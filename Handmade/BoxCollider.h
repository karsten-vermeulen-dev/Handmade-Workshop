#pragma once
#include "GLAD/gl.h"
#include <glm.hpp>

//TODO - Add some kind of 'central system' to render all colliders in the scene

class OBBCollider;
class SphereCollider;

class BoxCollider
{

public:

	const glm::vec3& GetScale() const;
	const glm::vec3& GetPosition() const;
	const glm::vec3& GetDimension() const;

	void SetScale(const glm::vec3& scale);
	void SetScale(GLfloat x, GLfloat y, GLfloat z);

	void SetPosition(const glm::vec3& position);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);

	void SetDimension(const glm::vec3& dimension);
	void SetDimension(GLfloat width, GLfloat height, GLfloat depth);

	bool IsColliding(const glm::vec3& point) const;
	bool IsColliding(GLfloat x, GLfloat y, GLfloat z) const;

	bool IsColliding(const OBBCollider& secondBox) const;
	bool IsColliding(const BoxCollider& secondBox) const;
	bool IsColliding(const SphereCollider& secondSphere) const;

	glm::vec3 PointOnBox(const glm::vec3& point) const;
	glm::vec3 PointOnBox(GLfloat x, GLfloat y, GLfloat z) const;

	void Update();
	void Render() { /*debug only*/ }

private:

	glm::vec3 min{ 0.0f };
	glm::vec3 max{ 0.0f };
	glm::vec3 scale{ 1.0f };
	glm::vec3 position{ 0.0f };
	glm::vec3 dimension{ 1.0f };
	glm::vec3 halfDimension{ 0.0f };
	glm::vec4 color{ 1.0f, 0.0f, 0.0f, 0.4f };

};