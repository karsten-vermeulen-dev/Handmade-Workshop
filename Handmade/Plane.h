#pragma once
#include "GLAD/gl.h"
#include <glm.hpp>
#include "LineSegment.h"
#include "SphereCollider.h"

struct PlanePoints
{
	glm::vec3 point1{ 0.0f };
	glm::vec3 point2{ 0.0f };
	glm::vec3 point3{ 0.0f };
	glm::vec3 point4{ 0.0f };
};

class Plane
{

public:

	enum class PlaneSide
	{
		OnPlane,
		Positive,
		Negative
	};

	void SetPoint1(const glm::vec3& point1);
	void SetPoint1(GLfloat x, GLfloat y, GLfloat z);

	void SetPoint2(const glm::vec3& point2);
	void SetPoint2(GLfloat x, GLfloat y, GLfloat z);

	void SetPoint3(const glm::vec3& point3);
	void SetPoint3(GLfloat x, GLfloat y, GLfloat z);

	void SetPoint4(const glm::vec3& point4);
	void SetPoint4(GLfloat x, GLfloat y, GLfloat z);

	void SetPosition(const glm::vec3& position);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);

	bool IsColliding(const SphereCollider& secondSphere) const;

	PlaneSide SideOfPlane(const glm::vec3& point) const;
	PlaneSide SideOfPlane(GLfloat x, GLfloat y, GLfloat z) const;

	GLfloat DistanceFromPlane(const glm::vec3& point) const;
	GLfloat DistanceFromPlane(GLfloat x, GLfloat y, GLfloat z) const;
	GLfloat DistanceFromPlane(const LineSegment& lineSegment) const;

	void Update();
	void Render() { /*debug only*/ }

private:

	PlanePoints planePoints;
	GLfloat distanceFromOrigin{ 0.0f };
	
	glm::vec3 normal{ 0.0f };
	glm::vec3 position{ 0.0f };
	glm::vec4 color{ 0.0f, 0.0f, 1.0f, 1.0f };

};