#pragma once
#include "GLAD/gl.h"
#include <glm.hpp>
#include <gtc/quaternion.hpp>

class RigidBody
{

public:

	static glm::vec3 GravityForce(GLdouble mass_1,
		GLdouble mass_2, const glm::vec3& distance);
	static glm::vec3 TorqueForce(const glm::vec3& force,
		const glm::vec3& contactPosition, const glm::vec3& centreOfMass);

	RigidBody();

	GLdouble GetAngle() const;
	GLdouble GetAngularVelocity() const;

	const glm::vec3& GetForce() const;
	const glm::vec3& GetTorque() const;
	const glm::vec3& GetPosition() const;
	const glm::vec3& GetVelocity() const;

	void SetMass(GLdouble mass);
	void SetAngle(GLdouble angle);
	void SetAngularMass(GLdouble mass);
	void SetAngularVelocity(GLdouble velocity);

	void SetForce(const glm::vec3& force);
	void SetTorque(const glm::vec3& torque);
	void SetPosition(const glm::vec3& position);
	void SetVelocity(const glm::vec3& velocity);

	void AddForce(const glm::vec3& force);
	void AddTorque(const glm::vec3& force, const glm::vec3& contactPoint);

	void Update(GLfloat deltaTime);

private:

	GLdouble mass{ 0.0 };
	GLdouble angle{ 0.0 };
	GLdouble angularMass{ 0.0 };
	GLdouble angularVelocity{ 0.0 };
	GLdouble angularAcceleration{ 0.0 };

	glm::vec3 force{ 0.0f };
	glm::vec3 torque{ 0.0f };
	glm::vec3 position{ 0.0f };
	glm::vec3 velocity{ 0.0f };
	glm::vec3 acceleration{ 0.0f };
	glm::quat orientation{ 1.0f, 0.0f, 0.0f, 0.0f };

	//TODO - Fix this later
	//glm::vec3 angularVelocity;
	//glm::vec3 angularAcceleration;
};