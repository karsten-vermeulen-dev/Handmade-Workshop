#include <assert.h>
#include "LineSegment.h"
#include "Plane.h"
#include "SphereCollider.h"

//======================================================================================================
void LineSegment::SetScale(GLfloat scale)
{
	this->scale = scale;
}
//======================================================================================================
void LineSegment::SetStartPoint(const glm::vec3& startPoint)
{
	this->startPoint = startPoint;
}
//======================================================================================================
void LineSegment::SetStartPoint(GLfloat x, GLfloat y, GLfloat z)
{
	startPoint.x = x;
	startPoint.y = y;
	startPoint.z = z;
}
//======================================================================================================
void LineSegment::SetEndPoint(const glm::vec3& endPoint)
{
	this->endPoint = endPoint;
}
//======================================================================================================
void LineSegment::SetEndPoint(GLfloat x, GLfloat y, GLfloat z)
{
	endPoint.x = x;
	endPoint.y = y;
	endPoint.z = z;
}
//======================================================================================================
bool LineSegment::IsColliding(const SphereCollider& secondSphere) const
{
	//TODO - Should the radius not be scaled?
	return (glm::length(secondSphere.GetPosition() - PointOnLine(secondSphere.GetPosition())) <=
		secondSphere.GetRadius());
}
//======================================================================================================
bool LineSegment::IsColliding(const Plane& secondPlane) const
{
	//TODO - Complete this
	//The line seg intersects the plane if the end points
	//are on different sides, (or are on the plane)
	return false;
}
//======================================================================================================
glm::vec3 LineSegment::PointOnLine(const glm::vec3& point) const
{
	return PointOnLine(point.x, point.y, point.z);
}
//======================================================================================================
glm::vec3 LineSegment::PointOnLine(GLfloat x, GLfloat y, GLfloat z) const
{
	auto lineSegment = endPoint - startPoint;
	assert(lineSegment != glm::vec3(0.0f));

	auto dot = glm::dot(lineSegment,
		(glm::vec3(x, y, z) - startPoint)) / glm::dot(lineSegment, lineSegment);

	//Either the point is on either end of the 
	//line segment or its a value between 0 and 1 
	//meaning that its on the line somewhere

	if (dot <= 0.0f)
	{
		return startPoint;
	}

	else if (dot >= 1.0f)
	{
		return endPoint;
	}

	else
	{
		return (startPoint + dot * lineSegment);
	}
}