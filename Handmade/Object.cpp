#include <assert.h>
#include <deque>
#include "Object.h"
#include "Shader.h"

//======================================================================================================
Object::Object(const std::string& tag) : tag(tag) 
{
	material.SetAmbient(0.4f, 0.4f, 0.4f);
	material.SetDiffuse(0.75f, 0.75f, 0.75f);
	material.SetSpecular(0.25, 0.25f, 0.25f);
}
//======================================================================================================
bool Object::IsLit() const
{
	return isLit;
}
//======================================================================================================
bool Object::IsAlive() const
{
	return isAlive;
}
//======================================================================================================
bool Object::IsActive() const
{
	return isActive;
}
//======================================================================================================
bool Object::IsVisible() const
{
	return isVisible;
}
//======================================================================================================
bool Object::IsTextured() const
{
	return isTextured;
}
//======================================================================================================
void Object::IsLit(bool flag)
{
	isLit = flag;
}
//======================================================================================================
void Object::IsAlive(bool flag)
{
	isAlive = flag;
}
//======================================================================================================
void Object::IsActive(bool flag)
{
	isActive = flag;
}
//======================================================================================================
void Object::IsVisible(bool flag)
{
	isVisible = flag;
}
//======================================================================================================
void Object::IsTextured(bool flag)
{
	isTextured = flag;
}
//======================================================================================================
GLuint Object::GetPriority() const
{
	return priority;
}
//======================================================================================================
const std::string& Object::GetTag() const
{
	return tag;
}
//======================================================================================================
const glm::vec4& Object::GetColor() const
{
	return color;
}
//======================================================================================================
const std::vector<Object*>& Object::GetChildren() const
{
	return children;
}
//======================================================================================================
Transform& Object::GetTransform()
{
	return transform;
}
//======================================================================================================
void Object::SetPriority(GLuint priority)
{
	this->priority = priority;
}
//======================================================================================================
void Object::SetTag(const std::string& tag)
{
	this->tag = tag;
}
//======================================================================================================
void Object::AddChild(Object* child)
{
	assert(child->parent == nullptr);
	children.emplace_back(child);
	children.back()->parent = this;
}
//======================================================================================================
glm::mat4 Object::GetFinalMatrix()
{
	std::deque<glm::mat4> matrices;
	matrices.emplace_front(transform.GetMatrix());

	auto parent = this->parent;

	while (parent)
	{
		matrices.emplace_front(parent->GetTransform().GetMatrix());
		parent = parent->parent;
	}

	auto finalMatrix = glm::mat4(1.0f);

	for (auto& matrix : matrices)
	{
		finalMatrix *= matrix;
	}

	return finalMatrix;
}