#include <algorithm>
#include "OBBCollider.h"

//======================================================================================================
const glm::vec3& OBBCollider::GetScale() const
{
	return scale;
}
//======================================================================================================
const glm::vec3& OBBCollider::GetPosition() const
{
	return position;
}
//======================================================================================================
const glm::quat& OBBCollider::GetRotation() const
{
	return rotation;
}
//======================================================================================================
const glm::vec3& OBBCollider::GetDimension() const
{
	return dimension;
}
//======================================================================================================
void OBBCollider::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
}
//======================================================================================================
void OBBCollider::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}
//======================================================================================================
void OBBCollider::SetPosition(const glm::vec3& position)
{
	this->position = position;
}
//======================================================================================================
void OBBCollider::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}
//======================================================================================================
void OBBCollider::SetRotation(const glm::quat& rotation)
{
	this->rotation = rotation;
}
//======================================================================================================
void OBBCollider::SetRotation(const glm::vec3& eulerAngles)
{
	rotation = glm::quat(glm::radians(eulerAngles));
}
//======================================================================================================
void OBBCollider::SetRotation(GLfloat pitch, GLfloat yaw, GLfloat roll)
{
	rotation = glm::quat(glm::radians(glm::vec3(pitch, yaw, roll)));
}
//======================================================================================================
void OBBCollider::SetDimension(const glm::vec3& dimension)
{
	this->dimension = dimension;
}
//======================================================================================================
void OBBCollider::SetDimension(GLfloat width, GLfloat height, GLfloat depth)
{
	dimension.x = width;
	dimension.y = height;
	dimension.z = depth;
}
//======================================================================================================
bool OBBCollider::IsColliding(const OBBCollider& secondBox) const
{
	//Assign all fifteen temporary axes based on both boxes' up, right and forward axes 
	//These will be the fifteen axes that all sixteen corners are projected onto
	//Here we have considered the extra nine axes needed to find the disjoint (if any)
	glm::vec3 axes[15];

	axes[0] = rightAxis;
	axes[1] = upAxis;
	axes[2] = forwardAxis;

	axes[3] = secondBox.rightAxis;
	axes[4] = secondBox.upAxis;
	axes[5] = secondBox.forwardAxis;

	axes[6] = glm::cross(rightAxis, secondBox.rightAxis);
	axes[7] = glm::cross(rightAxis, secondBox.upAxis);
	axes[8] = glm::cross(rightAxis, secondBox.forwardAxis);

	axes[9] = glm::cross(upAxis, secondBox.rightAxis);
	axes[10] = glm::cross(upAxis, secondBox.upAxis);
	axes[11] = glm::cross(upAxis, secondBox.forwardAxis);

	axes[12] = glm::cross(forwardAxis, secondBox.rightAxis);
	axes[13] = glm::cross(forwardAxis, secondBox.upAxis);
	axes[14] = glm::cross(forwardAxis, secondBox.forwardAxis);

	for (int i = 0; i < 15; i++)
	{
		//Check if one of the axis is 0, as this may result from a 
		//cross product of two vectors pointing in same direction
		//If a zero axis is found skip this loop iteration as its useless
		if (axes[i] == glm::vec3(0.0f)) continue;

		//Reset the min and max extent values each time a new axis is tested
		//Set both min and max to the first projected corner of each box
		glm::vec2 extents[2];
		extents[0].x = extents[0].y = glm::dot(corners[0], glm::normalize(axes[i]));
		extents[1].x = extents[1].y = glm::dot(secondBox.corners[0], glm::normalize(axes[i]));

		//Add both potentially colliding boxes into a temporary array
		//so that we can use the array when looping through both boxes
		OBBCollider colliders[2];
		colliders[0] = *this;
		colliders[1] = secondBox;

		//Now loop through each box...
		for (int j = 0; j < 2; j++)
		{
			//...and through each corner
			for (int k = 0; k < 8; k++)
			{
				//Project each box's corner onto the current active axis
				GLfloat projection = glm::dot(colliders[j].corners[k], glm::normalize(axes[i]));

				//Find the min and max extent values for each box
				extents[j].x = std::min(projection, extents[j].x);
				extents[j].y = std::max(projection, extents[j].y);
			}
		}

		//If there is at least 1 extent non-overlap found
		//boxes do not collide, so return false immediately
		if (!(extents[0].y > extents[1].x && extents[1].y > extents[0].x))
		{
			return false;
		}
	}

	//Otherwise there are extent overlaps for each axis
	//and that means both boxes collide, so return true
	return true;
}
//======================================================================================================
bool OBBCollider::IsColliding(const BoxCollider& secondBox) const
{
	OBBCollider collider;

	collider.SetScale(secondBox.GetScale());
	collider.SetPosition(secondBox.GetPosition());
	collider.SetDimension(secondBox.GetDimension());
	collider.Update();

	return IsColliding(collider);
}
//======================================================================================================
bool OBBCollider::IsColliding(const SphereCollider& secondSphere) const
{
	return (glm::length(secondSphere.GetPosition() - PointOnBox(secondSphere.GetPosition())) <=
		secondSphere.GetRadius());
}
//======================================================================================================
glm::vec3 OBBCollider::PointOnBox(const glm::vec3& point) const
{
	return PointOnBox(point.x, point.y, point.z);
}
//======================================================================================================
glm::vec3 OBBCollider::PointOnBox(GLfloat x, GLfloat y, GLfloat z) const
{
	glm::vec3 axes[3] = { rightAxis, upAxis, forwardAxis };
	glm::vec3 distanceFromObject = glm::vec3(x, y, z) - position;

	//Loop through all three axes and project
	//the distance vector onto current axis
	GLfloat clamp[3];
	glm::vec3 normalizedAxis[3];

	for (int i = 0; i < 3; i++)
	{
		normalizedAxis[i] = glm::normalize(axes[i]);
		GLfloat projection = glm::dot(distanceFromObject, normalizedAxis[i]);
		clamp[i] = glm::clamp(projection, -halfDimension[i], halfDimension[i]);
	}

	//The clamp values are used together with the axes to determine the exact
	//point on the edge of the box that lines up with the passed position point
	return    position + (normalizedAxis[0] * clamp[0])
		+ (normalizedAxis[1] * clamp[1])
		+ (normalizedAxis[2] * clamp[2]);
}
//======================================================================================================
void OBBCollider::Update()
{
	halfDimension = dimension * scale * 0.5f;

	//Use the up, right and forward vector to calculate box's corners 
	//below and to create projection vectors to project onto later on
	//We have to reset them to their defaults before rotating them
	upAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	rightAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	forwardAxis = glm::vec3(0.0f, 0.0f, -1.0f);

	//Transform the up, right and forward vectors based on how the OBB is rotated 
	glm::vec3 upAxis = rotation * upAxis * halfDimension.y;
	glm::vec3 rightAxis = rotation * rightAxis * halfDimension.x;
	glm::vec3 forwardAxis = rotation * forwardAxis * halfDimension.z;

	//Calculate all eight corners of the box based on bound's centre 
	//position, starting in the top left corner and moving clockwise

	//Front face
	corners[0] = position - rightAxis + upAxis - forwardAxis;
	corners[1] = position + rightAxis + upAxis - forwardAxis;
	corners[2] = position + rightAxis - upAxis - forwardAxis;
	corners[3] = position - rightAxis - upAxis - forwardAxis;

	//Back face
	corners[4] = position - rightAxis + upAxis + forwardAxis;
	corners[5] = position + rightAxis + upAxis + forwardAxis;
	corners[6] = position + rightAxis - upAxis + forwardAxis;
	corners[7] = position - rightAxis - upAxis + forwardAxis;
}

//======================================================================================================
//OLD code for use with 2D OBB
//======================================================================================================
//bool OBB2D::IsColliding(const OBB2D* tempBoxes) const
//{
//	float extentMin[2];
//	float extentMax[2];
//	float projection = 0.0f;
//
//	glm::vec2 tempAxis[4];
//	glm::vec2 normalizedAxis;
//
//	tempAxis[0] = tempBoxes[0].   rightAxis;
//	tempAxis[1] = tempBoxes[0].   upAxis;
//	tempAxis[2] = tempBoxes[1].   rightAxis;
//	tempAxis[3] = tempBoxes[1].   upAxis;
//
//	for (int i = 0; i < 4; i++)
//	{
//		normalizedAxis = glm::normalize(tempAxis[i]);
//
//		extentMin[0] = extentMax[0] = glm::dot(tempBoxes[0].   corners[0], normalizedAxis);
//		extentMin[1] = extentMax[1] = glm::dot(tempBoxes[1].   corners[0], normalizedAxis);
//
//		for (int j = 0; j < 2; j++)
//		{
//			for (int k = 0; k < 4; k++)
//			{
//				projection = glm::dot(tempBoxes[j].   corners[k], normalizedAxis);
//				extentMin[j] = std::min(projection, extentMin[j]);
//				extentMax[j] = std::max(projection, extentMax[j]);
//			}
//		}
//
//		if (!(extentMax[0] > extentMin[1] && extentMax[1] > extentMin[0]))
//		{
//			return false;
//		}
//
//	}
//
//	return true;
//}