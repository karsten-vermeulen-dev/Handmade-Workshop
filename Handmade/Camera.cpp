#include <assert.h>
#include <gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Screen.h"
#include "Shader.h"

//======================================================================================================
Camera::Camera(const std::string& tag) : Object(tag) {}
//======================================================================================================
const glm::ivec2 Camera::GetResolution() const
{
	return resolution;
}
//======================================================================================================
void Camera::SetVelocity(GLfloat velocity)
{
	velocity = velocity;
}
//======================================================================================================
void Camera::SetFieldOfView(GLfloat fieldOfView)
{
	fieldOfView = fieldOfView;
}
//======================================================================================================
void Camera::SetBackgroundColor(const glm::vec4& backgroundColor)
{
	this->backgroundColor = backgroundColor;
}
//======================================================================================================
void Camera::SetBackgroundColor(const glm::uvec4& backgroundColor)
{
	this->backgroundColor.r = backgroundColor.r / 255.0f;
	this->backgroundColor.g = backgroundColor.g / 255.0f;
	this->backgroundColor.b = backgroundColor.b / 255.0f;
	this->backgroundColor.a = backgroundColor.a / 255.0f;
}
//======================================================================================================
void Camera::SetBackgroundColor(GLuint r, GLuint g, GLuint b, GLuint a)
{
	backgroundColor.r = r / 255.0f;
	backgroundColor.g = g / 255.0f;
	backgroundColor.b = b / 255.0f;
	backgroundColor.a = a / 255.0f;
}
//======================================================================================================
void Camera::SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	backgroundColor.r = r;
	backgroundColor.g = g;
	backgroundColor.b = b;
	backgroundColor.a = a;
}
//======================================================================================================
void Camera::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	resolution.x = width;
	resolution.y = height;
	viewport = glm::ivec4(x, y, width, height);
}
//======================================================================================================
void Camera::CreatePerspView()
{
	Screen::Instance()->IsDepthTestEnabled(true);
	Screen::Instance()->SetColor(backgroundColor);
	Screen::Instance()->SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);

	GLfloat aspectRatio = (resolution.x) / static_cast<GLfloat>(resolution.y);

	projectionMatrix = glm::perspective(glm::radians(fieldOfView),
		aspectRatio, nearClip, farClip);
}
//======================================================================================================
void Camera::CreateOrthoView(Origin2D origin)
{
	Screen::Instance()->IsDepthTestEnabled(false);
	Screen::Instance()->SetColor(backgroundColor);
	Screen::Instance()->SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);

	if (origin == Origin2D::TopLeft)
	{
		projectionMatrix = glm::ortho(0.0f, static_cast<GLfloat>(resolution.x),
			static_cast<GLfloat>(resolution.y), 0.0f);
	}

	else if (origin == Origin2D::BottomLeft)
	{
		projectionMatrix = glm::ortho(0.0f, static_cast<GLfloat>(resolution.x),
			0.0f, static_cast<GLfloat>(resolution.y));
	}
}
//======================================================================================================
glm::vec2 Camera::ConvertWorldToScreen(const glm::vec3& worldPosition) const
{
	//convert to clip space
	glm::vec4 clipCoordinate = projectionMatrix * viewMatrix * glm::vec4(worldPosition, 1.0f);

	//convert to NDC
	clipCoordinate /= clipCoordinate.w;

	//convert to screen space
	return glm::vec2((clipCoordinate.x + 1.0f) * resolution.x * 0.5f,
		(clipCoordinate.y + 1.0f) * resolution.y * 0.5f);
}
//======================================================================================================
glm::vec3 Camera::ConvertScreenToWorld(const glm::vec2& screenPosition, GLfloat zNDC) const
{
	//TODO - Consider using 'glm::unProject()'
	//TODO - Replace ' resolution' with ' viewport'

	//convert to NDC
	glm::vec4 NDC((2.0f * screenPosition.x) / resolution.x - 1.0f,
		(2.0f * (resolution.y - screenPosition.y)) / resolution.y - 1.0f,
		zNDC, 1.0f);

	//convert to world space (4D)
	glm::vec4 worldCoordinate = glm::inverse(viewMatrix) * glm::inverse(projectionMatrix) * NDC;

	//convert to world space (3D)
	return (worldCoordinate /= worldCoordinate.w);
}
//======================================================================================================
void Camera::Reset()
{
	viewMatrix = glm::mat4(1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	projectionMatrix = glm::mat4(1.0f);
	transform.SetPosition(glm::vec3(0.0f));
}
//======================================================================================================
void Camera::SendToShader(Shader& shader)
{
	shader.SendData("view", viewMatrix);
	shader.SendData("projection", projectionMatrix);
}