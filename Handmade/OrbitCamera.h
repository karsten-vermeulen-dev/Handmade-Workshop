#pragma once
#include "Camera.h"

class OrbitCamera : public Camera
{

public:

	OrbitCamera(const std::string& tag);
	virtual ~OrbitCamera() {}

	void SetSensitivity(GLfloat sensitivity);
	void SetPitchRange(const glm::vec2& range);
	void SetPitchRange(GLfloat min, GLfloat max);

	void SetZoomDistanceRange(const glm::vec2& range);
	void SetZoomDistanceRange(GLfloat min, GLfloat max);

	void Zoom(GLint motion);
	void Rotate(const glm::ivec2& motion);
	void Rotate(GLint motionX, GLint motionY);

	virtual void Render(Shader& shader) {};
	virtual void Update(GLfloat deltaTime);
	virtual void SendToShader(Shader& shader);

private:

	GLfloat sensitivity{ 0.0f };
	GLfloat zoomDistance{ 5.0f };

	glm::vec3 target{ 0.0f };
	glm::vec2 pitchRange{ -89.0f, 89.0f };
	glm::vec2 zoomDistanceRange{ 0.5f, 50.0f };

};