#include "Capsule.h"

//======================================================================================================
Capsule::Capsule(const glm::vec3& vertex1, const glm::vec3& vertex2, GLfloat radius)
{
	//TODO - Create line segment using passed vertices
	radius = radius;
}
//======================================================================================================
bool Capsule::IsColliding(const SphereCollider& secondSphere) const
{
	//TODO - This is still WIP

	//1. Get sq dist from sphere centre to the line seg
	GLfloat squaredDistance = 0.0f; //lineseg.SquaredDist(s.GetCentre());

	//2. Work out squared sum of radii (r1 + r2) * (r1 + r2)
	//NOT: (r1 * r1) + (r2 * r2)

	//3. We do intersect if sq dist < sq sum of radii
	return squaredDistance < (radius + secondSphere.GetRadius())
		* (radius + secondSphere.GetRadius());
}