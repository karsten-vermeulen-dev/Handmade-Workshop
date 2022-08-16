#pragma once
#include "Object.h"

class Camera : public Object
{

public:

	enum class Origin2D
	{
		TopLeft,
		BottomLeft
	};

	Camera(const std::string& tag);
	virtual ~Camera() = 0 {}

	const glm::ivec2 GetResolution() const;

	void SetVelocity(GLfloat velocity);
	void SetFieldOfView(GLfloat fieldOfView);
	void SetViewport(GLint x, GLint y, GLsizei width, GLsizei height);

	void SetBackgroundColor(const glm::vec4& backgroundColor);
	void SetBackgroundColor(const glm::uvec4& backgroundColor);
	void SetBackgroundColor(GLuint r, GLuint g, GLuint b, GLuint a = 1U);
	void SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	void CreatePerspView();
	void CreateOrthoView(Origin2D origin = Origin2D::BottomLeft);

	glm::vec2 ConvertWorldToScreen(const glm::vec3& worldPosition) const;
	glm::vec3 ConvertScreenToWorld(const glm::vec2& screenPosition, GLfloat zNDC) const;

	void Reset();

	virtual void Render(Shader& shader) = 0;
	virtual void Update(GLfloat deltaTime) = 0;
	virtual void SendToShader(Shader& shader);

protected:

	const GLfloat minFOV{ 1.0f };
	const GLfloat maxFOV{ 100.0f };
	const GLfloat nearClip{ 0.1f };
	const GLfloat farClip{ 1000.0f };

	GLfloat velocity{ 0.0f };
	GLfloat fieldOfView{ 45.0f };

	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 forward{ 0.0f, 0.0f, -1.0f };

	glm::ivec4 viewport{ 0 };
	glm::ivec2 resolution{ 0 };
	glm::vec4 backgroundColor{ 0.0f };

	glm::mat4 viewMatrix{ 1.0f };
	glm::mat4 projectionMatrix{ 1.0f };

};