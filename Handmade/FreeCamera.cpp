#include "FreeCamera.h"
#include "Input.h"
#include "Shader.h"

//======================================================================================================
FreeCamera::FreeCamera(const std::string& tag) : Camera(tag)
{
	//We set the yaw to an initial 90 degrees so that the
	//forward vector calculations later result in (0, 0, -1)
	transform.SetRotation(0.0f, -90.0f, 0.0f);
}
//======================================================================================================
void FreeCamera::IsFlying(bool isFlying)
{
	this->isFlying = isFlying;
}
//======================================================================================================
void FreeCamera::SetSensitivity(GLfloat sensitivity)
{
	this->sensitivity = sensitivity;
}
//======================================================================================================
void FreeCamera::SetClampedYPosition(GLfloat position)
{
	clampedYPosition = position;
}
//======================================================================================================
void FreeCamera::SetPitchRange(const glm::vec2& range)
{
	SetPitchRange(range.x, range.y);
}
//======================================================================================================
void FreeCamera::SetPitchRange(GLfloat min, GLfloat max)
{
	pitchRange.x = min;
	pitchRange.y = max;
}
//======================================================================================================
void FreeCamera::MoveUp()
{
	if (isFlying)
	{
		glm::vec3 right = glm::cross(forward, up);
		glm::vec3 up = glm::normalize(glm::cross(forward, right));
		transform.Translate(-up * velocity, Transform::Space::Global);
	}
}
//======================================================================================================
void FreeCamera::MoveDown()
{
	if (isFlying)
	{
		glm::vec3 right = glm::cross(forward, up);
		glm::vec3 up = glm::normalize(glm::cross(forward, right));
		transform.Translate(up * velocity, Transform::Space::Global);
	}
}
//======================================================================================================
void FreeCamera::MoveLeft()
{
	transform.Translate(glm::normalize(glm::cross(-forward, up)) * velocity,
		Transform::Space::Global);
}
//======================================================================================================
void FreeCamera::MoveRight()
{
	transform.Translate(glm::normalize(glm::cross(forward, up)) * velocity,
		Transform::Space::Global);
}
//======================================================================================================
void FreeCamera::MoveForward()
{
	transform.Translate(forward * velocity, Transform::Space::Global);
}
//======================================================================================================
void FreeCamera::MoveBackward()
{
	transform.Translate(-forward * velocity, Transform::Space::Global);
}
//======================================================================================================
void FreeCamera::Zoom(GLint motion)
{
	fieldOfView += -motion;
	fieldOfView = glm::clamp(fieldOfView, minFOV, maxFOV);
	CreatePerspView();
}
//======================================================================================================
void FreeCamera::Rotate(const glm::ivec2& motion)
{
	Rotate(motion.x, motion.y);
}
//======================================================================================================
void FreeCamera::Rotate(GLint motionX, GLint motionY)
{
	auto rotation = transform.GetEulerAngles();

	rotation.y += motionX * sensitivity;
	rotation.x += -motionY * sensitivity;
	rotation.x = glm::clamp(rotation.x, pitchRange.x, pitchRange.y);

	transform.SetRotation(rotation);
}
//======================================================================================================
void FreeCamera::Update(GLfloat deltaTime)
{
	if (!isFlying)
	{
		auto position = transform.GetPosition();
		position.y = clampedYPosition;
		transform.SetPosition(position);
	}

	//TODO - Fix the camera so that if not in 'fly' mode the camera still moves correctly
	//If camera is in debug/fly mode, transform the forward vector based on
	//the overall rotation, so that camera moves forward in any direction
	// forward =  lookAt;

	//Otherwise ignore the y component so that the forward vector stays on a horizontal plane
	//when moving. This prevents the camera from moving forward freely and will restrict it. 
	//The camera still looks anywhere but moves with restrictions
	if (!isFlying)
	{
		forward.y = 0.0f;
	}

	//When the camera moves around the y-axis (yaw) the vector changes along the x/z plane
	//The x-axis (pitch) movements only affect the y component of the forward vector 
	auto rotation = transform.GetEulerAngles();
	forward.x = glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x));
	forward.y = glm::sin(glm::radians(rotation.x));
	forward.z = glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x));
}
//======================================================================================================
void FreeCamera::SendToShader(Shader& shader)
{
	viewMatrix = glm::lookAt(transform.GetPosition(),
		transform.GetPosition() + forward, up);

	Camera::SendToShader(shader);
}

//======================================================================================================
//OLD code that makes use of rotation matrices - not ideal but good for learning
//======================================================================================================
//void FreeCamera::RotateMatrix()
//{
//	//create rotation matrices for x/y rotation
//	//we always start with fresh new matrices
//	 rotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians( yaw), glm::vec3(0.0f, -1.0f, 0.0f));
//	 rotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians( pitch), glm::vec3(-1.0f, 0.0f, 0.0f));
//
//	//create a total rotation transformation based on x and y rotations
//	//we do a y rotation FIRST and then a local x rotation thereafter
//	glm::mat4 totalRotation =  rotationMatrixY *  rotationMatrixX;
//
	//create a separate move rotation based on if camera is in freeflow mode or not
	//this affects movement so that camera either moves freely or is bound to a Y plane 
	//glm::mat4 moveRotation = ( isFreeFlow) ? totalRotation :  rotationMatrixY;

	//apply same rotation to move direction vector so that
	//the camera moves correctly based on camera orientation
	// moveDirection = glm::vec3(moveRotation * glm::vec4( moveDirection, 1.0f));

//	//calculate the FreeCamera's viewing vector by transforming the 
//	//initial viewing target based on FreeCamera's total rotation
//	 lookAt = glm::vec3(totalRotation * glm::vec4( target, 1.0f));
//
//	//apply total rotation to move direction vector for correct movement
//	moveDirection = glm::vec3(totalRotation * glm::vec4(moveDirection, 1.0f));

	////update camera position based on move direction only if camera is set to move
	//if (moveDirection != glm::vec3(0.0f))
	//{
	//	 position += moveDirection *  velocity;
	//}

	////update camera's view matrix
	// viewMatrix = glm::lookAt( position,  position + lookAtDirection, glm::vec3(0.0f, 1.0f, 0.0f));

//	//if camera is in debug/fly mode, transform the forward vector based on
//	//the overall rotation, so that camera moves forward in any direction
//	if ( isFlying)
//	{
//		 forward =  lookAt;
//	}
//
//	//otherwise transform the forward vector based on ONLY the y-rotations as
//	//this prevents the camera from moving forward freely and will restrict it to
//	//a certain y-axis. The camera still looks anywhere but moves with restrictions
//	else
//	{
//		 forward = glm::vec3( rotationMatrixY * glm::vec4( target, 1.0f));
//	}
//
//	//calculate the new right vector based on the forward vector
//	//so that camera can move local left/right based on rotation
//	 right = glm::normalize(glm::cross( forward,  up));
//}

//=============================================================================
//function that controls rotation of camera around local X axis
//=============================================================================
//void Camera::RotateX(short motionY)
//{
//
//	//generate a rotation angle based on mouse motion and rotation sensitivity
//	GLfloat angle = motionY *  sensitivity;
//
//	//calculate the dot product between the "look at" vector and the up vector. This will
//	//give us a value to determine how far up or down the X axis the camera is tilting
//	double dot = glm::dot( lookAt,  upVector);
//
//	//if the camera is tilting upwards or downwards and only if it is between its allowed 
//	//threshold, then accumulate the current X axis rotation using the angle calculated
//	if ((motionY > 0 && dot >=  threshold.x) || (motionY < 0 && dot <=  threshold.y))
//	{
//		 rotationMatrixX = glm::rotate( rotationMatrixX,
//			glm::radians(angle),
//			glm::vec3(-1.0f, 0.0f, 0.0f));
//	}
//
//}

//=============================================================================
//function that controls rotation of camera around global Y axis
//=============================================================================
//void Camera::RotateY(short motionX)
//{
//
//	//generate a rotation angle based on mouse motion and rotation sensitivity
//	GLfloat angle = motionX *  sensitivity;
//
//	//accumulate the current Y axis rotation using the angle calculated
//	 rotationMatrixY = glm::rotate( rotationMatrixY,
//		glm::radians(angle),
//		glm::vec3(0.0f, -1.0f, 0.0f));
//
//}