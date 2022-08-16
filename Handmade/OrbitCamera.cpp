#include <gtc\matrix_transform.hpp>
#include "Input.h"
#include "OrbitCamera.h"
#include "Shader.h"

//======================================================================================================
OrbitCamera::OrbitCamera(const std::string& tag) : Camera(tag) {}
//======================================================================================================
void OrbitCamera::SetSensitivity(GLfloat sensitivity)
{
	this->sensitivity = sensitivity;
}
//======================================================================================================
void OrbitCamera::SetPitchRange(const glm::vec2& range)
{
	SetPitchRange(range.x, range.y);
}
//======================================================================================================
void OrbitCamera::SetPitchRange(GLfloat min, GLfloat max)
{
	pitchRange.x = min;
	pitchRange.y = max;
}
//======================================================================================================
void OrbitCamera::SetZoomDistanceRange(GLfloat min, GLfloat max)
{
	zoomDistanceRange.x = min;
	zoomDistanceRange.y = max;
}
//======================================================================================================
void OrbitCamera::SetZoomDistanceRange(const glm::vec2& range)
{
	SetZoomDistanceRange(range.x, range.y);
}
//======================================================================================================
void OrbitCamera::Zoom(GLint motion)
{
	zoomDistance -= motion;
	zoomDistance = glm::clamp(zoomDistance, zoomDistanceRange.x, zoomDistanceRange.y);
}
//======================================================================================================
void OrbitCamera::Rotate(const glm::ivec2& motion)
{
	Rotate(motion.x, motion.y);
}
//======================================================================================================
void OrbitCamera::Rotate(GLint motionX, GLint motionY)
{
	auto rotation = transform.GetEulerAngles();

	//The y rotation is the azimuth and the x rotation is the elevation
	rotation.y += motionX * sensitivity;
	rotation.x += -motionY * sensitivity;
	rotation.x = glm::clamp(rotation.x, pitchRange.x, pitchRange.y);

	transform.SetRotation(rotation);
}
//======================================================================================================
void OrbitCamera::SendToShader(Shader& shader)
{
	viewMatrix = glm::lookAt(transform.GetPosition(), target, up);
	Camera::SendToShader(shader);
}
//======================================================================================================
void OrbitCamera::Update(GLfloat deltaTime)
{
	glm::vec3 position;

	auto rotation = transform.GetEulerAngles();
	position.x = glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)) * zoomDistance;
	position.y = glm::sin(glm::radians(rotation.x)) * zoomDistance;
	position.z = glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)) * zoomDistance;

	transform.SetPosition(position);
}