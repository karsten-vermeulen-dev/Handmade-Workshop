#pragma once
#include "Camera.h"

class FreeCamera : public Camera
{

public:

	FreeCamera(const std::string& tag);
	virtual ~FreeCamera() {}

	void IsFlying(bool isFlying);
	void SetSensitivity(GLfloat sensitivity);
	void SetClampedYPosition(GLfloat position);
	void SetPitchRange(const glm::vec2& range);
	void SetPitchRange(GLfloat min, GLfloat max);

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void Zoom(GLint motion);
	void Rotate(const glm::ivec2& motion);
	void Rotate(GLint motionX, GLint motionY);

	virtual void Render(Shader& shader) {};
	virtual void Update(GLfloat deltaTime);
	virtual void SendToShader(Shader& shader);

private:

	bool isFlying{ true };
	GLfloat sensitivity{ 0.0f };
	GLfloat clampedYPosition{ 1.0f };
	glm::vec2 pitchRange{ -89.0f, 89.0f };

};