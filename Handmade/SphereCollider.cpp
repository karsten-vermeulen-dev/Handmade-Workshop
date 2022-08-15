#include <algorithm>
#include "SphereCollider.h"

//======================================================================================================
SphereCollider::SphereCollider() {}
//======================================================================================================
float SphereCollider::GetScale() const
{
	return scale;
}
//======================================================================================================
float SphereCollider::GetRadius() const
{
	return radius;
}
//======================================================================================================
float SphereCollider::GetRadiusScaled() const
{
	return radiusScaled;
}
//======================================================================================================
const glm::vec3& SphereCollider::GetPosition() const
{
	return position;
}
//======================================================================================================
void SphereCollider::SetScale(GLfloat scale)
{
	this->scale = scale;
}
//======================================================================================================
void SphereCollider::SetRadius(GLfloat radius)
{
	this->radius = radius;
}
//======================================================================================================
void SphereCollider::SetPosition(const glm::vec3& position)
{
	this->position = position;
}
//======================================================================================================
void SphereCollider::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}
//======================================================================================================
bool SphereCollider::IsColliding(const BoxCollider& secondBox) const
{
	return secondBox.IsColliding(*this);
}
//======================================================================================================
bool SphereCollider::IsColliding(const LineSegment& secondLine) const
{
	return secondLine.IsColliding(*this);
}
//======================================================================================================
bool SphereCollider::IsColliding(const SphereCollider& secondSphere) const
{
	return (glm::length(position - secondSphere.position) <=
		(radius + secondSphere.radius));
}
//======================================================================================================
void SphereCollider::Update()
{
	radiusScaled = radius * scale;
}