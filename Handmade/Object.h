#pragma once
#include "GLAD/gl.h"
#include <glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "Material.h"
#include "Transform.h"

class Object
{

public:

	Object(const std::string& tag);
	virtual ~Object() = 0 {}

	bool IsLit() const;
	bool IsAlive() const;
	bool IsActive() const;
	bool IsVisible() const;
	bool IsTextured() const;

	void IsLit(bool flag);
	void IsAlive(bool flag);
	void IsActive(bool flag);
	void IsVisible(bool flag);
	void IsTextured(bool flag);

	GLuint GetPriority() const;
	const std::string& GetTag() const;
	const glm::vec4& GetColor() const;
	const std::vector<Object*>& GetChildren() const;

	Transform& GetTransform();

	void SetPriority(GLuint priority);
	void SetTag(const std::string& tag);

	void AddChild(Object* child);

	virtual void SetColor(const glm::vec4& color) {}
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {}

	virtual void Render(Shader& shader) = 0;
	virtual void Update(GLfloat deltaTime) = 0;
	virtual void SendToShader(Shader& shader) = 0;

protected:

	glm::mat4 GetFinalMatrix();

	bool isLit{ false };
	bool isAlive{ true };
	bool isActive{ true };
	bool isVisible{ true };
	bool isTextured{ false };

	GLuint priority{ 0 };
	glm::vec4 color{ 1.0f };
	glm::mat3 normalMatrix{ 1.0f };

	std::string tag;
	Material material;
	Transform transform;

	Object* parent{ nullptr };
	std::vector<Object*> children;

};