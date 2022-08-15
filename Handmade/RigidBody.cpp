#include "RigidBody.h"

//======================================================================================================
glm::vec3 RigidBody::GravityForce(GLdouble mass_1, GLdouble mass_2, const glm::vec3& distance)
{
	const GLdouble gravity = 1.0000000000667;
	GLdouble gravityForce = (gravity * mass_1 * mass_2) /
		glm::length(distance) * glm::length(distance);
	return glm::normalize(distance) * static_cast<GLfloat>(gravityForce);
}
//======================================================================================================
glm::vec3 RigidBody::TorqueForce(const glm::vec3& force,
	const glm::vec3& contactPosition, const glm::vec3& centreOfMass)
{
	//The result produces a vector, used for the rotation axis and torque magnitude
	return glm::cross(force, (centreOfMass - contactPosition));
}
//======================================================================================================
RigidBody::RigidBody() {}
//======================================================================================================
GLdouble RigidBody::GetAngle() const
{
	return angle;
}
//======================================================================================================
GLdouble RigidBody::GetAngularVelocity() const
{
	return angularVelocity;
}
//======================================================================================================
const glm::vec3& RigidBody::GetForce() const
{
	return force;
}
//======================================================================================================
const glm::vec3& RigidBody::GetTorque() const
{
	return torque;
}
//======================================================================================================
const glm::vec3& RigidBody::GetPosition() const
{
	return position;
}
//======================================================================================================
const glm::vec3& RigidBody::GetVelocity() const
{
	return velocity;
}
//======================================================================================================
void RigidBody::SetMass(GLdouble mass)
{
	this->mass = mass;
}
//======================================================================================================
void RigidBody::SetAngle(GLdouble angle)
{
	this->angle = angle;
}
//======================================================================================================
void RigidBody::SetAngularMass(GLdouble mass)
{
	angularMass = mass;
}
//======================================================================================================
void RigidBody::SetAngularVelocity(GLdouble velocity)
{
	angularVelocity = velocity;
}
//======================================================================================================
void RigidBody::SetForce(const glm::vec3& force)
{
	this->force = force;
}
//======================================================================================================
void RigidBody::SetTorque(const glm::vec3& torque)
{
	this->torque = torque;
}
//======================================================================================================
void RigidBody::SetPosition(const glm::vec3& position)
{
	this->position = position;
}
//======================================================================================================
void RigidBody::SetVelocity(const glm::vec3& velocity)
{
	this->velocity = velocity;
}
//======================================================================================================
void RigidBody::AddForce(const glm::vec3& force)
{
	this->force += force;
}
//======================================================================================================
void RigidBody::AddTorque(const glm::vec3& force, const glm::vec3& contactPoint)
{
	AddForce(force);

	//T = F x d
	glm::vec3 distance = contactPoint - position;
	glm::vec3 torque = glm::cross(force, distance);
	torque += torque;
}
//======================================================================================================
void RigidBody::Update(GLfloat deltaTime)
{
	if (mass > 0.0)    acceleration = (force / static_cast<float>(mass));
	if (angularMass > 0.0)    angularAcceleration = glm::length(torque) / angularMass;

	glm::vec3 oldVelocity = velocity;
	velocity += acceleration * deltaTime;
	position += (velocity + oldVelocity) * 0.5f * deltaTime;

	GLdouble oldAngularVelocity = angularVelocity;
	angularVelocity += angularAcceleration * deltaTime;
	angle += (angularVelocity + oldAngularVelocity) * 0.5 * deltaTime;

	//Extra code to be sorted later (WIP)
	//Here we assume AngVel is a Vec3 object
	//angularAcceleration = torque;
	/*if (angVel.SqLen() > 0)
	{
		Vec3f axis = angVel;
		axis.Normalise();
		Quaternion rotThisFrame(axis, angVel.Len() * dt);
		quat = rotThisFrame * quat;
	}*/
}