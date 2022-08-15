#include <algorithm>
#include "BoxCollider.h"
#include "OBBCollider.h"
#include "SphereCollider.h"

//======================================================================================================
const glm::vec3& BoxCollider::GetScale() const
{
	return scale;
}
//======================================================================================================
const glm::vec3& BoxCollider::GetPosition() const
{
	return position;
}
//======================================================================================================
const glm::vec3& BoxCollider::GetDimension() const
{
	return dimension;
}
//======================================================================================================
void BoxCollider::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
}
//======================================================================================================--
void BoxCollider::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}
//======================================================================================================
void BoxCollider::SetPosition(const glm::vec3& position)
{
	this->position = position;
}
//======================================================================================================
void BoxCollider::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}
//======================================================================================================
void BoxCollider::SetDimension(const glm::vec3& dimension)
{
	this->dimension = dimension;
}
//======================================================================================================
void BoxCollider::SetDimension(GLfloat width, GLfloat height, GLfloat depth)
{
	dimension.x = width;
	dimension.y = height;
	dimension.z = depth;
}
//======================================================================================================
bool BoxCollider::IsColliding(const glm::vec3& point) const
{
	return IsColliding(point.x, point.y, point.z);
}
//======================================================================================================
bool BoxCollider::IsColliding(GLfloat x, GLfloat y, GLfloat z) const
{
	return ((x >= min.x && x <= max.x) &&
		(y >= min.y && y <= max.y) &&
		(z >= min.z && z <= max.z));
}
//======================================================================================================
bool BoxCollider::IsColliding(const OBBCollider& secondBox) const
{
	return secondBox.IsColliding(*this);
}
//======================================================================================================
bool BoxCollider::IsColliding(const BoxCollider& secondBox) const
{
	return ((max.x >= secondBox.min.x && secondBox.max.x >= min.x) &&
		(max.y >= secondBox.min.y && secondBox.max.y >= min.y) &&
		(max.z >= secondBox.min.z && secondBox.max.z >= min.z));
}
//======================================================================================================
bool BoxCollider::IsColliding(const SphereCollider& secondSphere) const
{
	return (glm::length(secondSphere.GetPosition() - PointOnBox(secondSphere.GetPosition())) <=
		secondSphere.GetRadius());
}
//======================================================================================================
glm::vec3 BoxCollider::PointOnBox(const glm::vec3& point) const
{
	return PointOnBox(point.x, point.y, point.z);
}
//======================================================================================================
glm::vec3 BoxCollider::PointOnBox(GLfloat x, GLfloat y, GLfloat z) const
{
	return position -
		glm::clamp((position - glm::vec3(x, y, z)), -halfDimension, halfDimension);
}
//======================================================================================================
void BoxCollider::Update()
{
	halfDimension = dimension * scale * 0.5f;

	min.x = position.x - halfDimension.x;
	min.y = position.y - halfDimension.y;
	min.z = position.z - halfDimension.z;
	max.x = position.x + halfDimension.x;
	max.y = position.y + halfDimension.y;
	max.z = position.z + halfDimension.z;
}