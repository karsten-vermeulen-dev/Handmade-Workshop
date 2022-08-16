#include <algorithm>
#include "Axes.h"
#include "Input.h"
#include "Shader.h"

GLuint Axes::totalObjects = 0;

//======================================================================================================
Axes::Axes(const std::string& tag, const std::string& filenameModel) : Object(tag), model(tag)
{
	model.Load(tag, filenameModel, true);
	model.SetModel(tag);
}
//======================================================================================================
Axes::Axes(const std::string& tag, GLint size, GLfloat lineWidth)
	: Object(tag), size(size), lineWidth(lineWidth), isPrimitive(true), model(tag), buffer(tag, 6)
{
	Create();
}
//======================================================================================================
Axes::~Axes()
{
	buffer.Destroy(tag);
}
//======================================================================================================
void Axes::SetSize(GLint size)
{
	size = size;
	size = std::max(size, 1);
	Create();
}
//======================================================================================================
void Axes::SetLineWidth(GLfloat lineWidth)
{
	this->lineWidth = lineWidth;
}
//======================================================================================================
const glm::vec3& Axes::GetArrowTipPositionX() const
{
	return arrowTipPositionX;
}
//======================================================================================================
const glm::vec3& Axes::GetArrowTipPositionY() const
{
	return arrowTipPositionY;
}
//======================================================================================================
const glm::vec3& Axes::GetArrowTipPositionZ() const
{
	return arrowTipPositionZ;
}
//======================================================================================================
void Axes::Render(Shader& shader)
{
	if (isPrimitive)
	{
		Buffer::SetLineWidth(lineWidth);

		//TODO - Find a way to do this only once
		buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
			Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::IntData);
		buffer.LinkVBO(shader.GetAttributeID("colorIn"),
			Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

		shader.SendData("isTextured", false);
		shader.SendData("model", transform.GetMatrix());

		buffer.Render(Buffer::RenderMode::Lines);
	}

	else
	{
		model.GetTransform() = transform;
		model.Render(shader);
	}

	arrowTipPositionX = transform.GetRotation() * glm::vec3(0.9f, 0.0f, 0.0f);
	arrowTipPositionY = transform.GetRotation() * glm::vec3(0.0f, 0.9f, 0.0f);
	arrowTipPositionZ = transform.GetRotation() * glm::vec3(0.0f, 0.0f, -0.9f);
}
//======================================================================================================
void Axes::Create()
{
	//We don't want to create new buffer 
	//objects everytime the axes are resized
	//TODO - Implement RAII properly

	GLint vertices[] = { -size, 0, 0,  size, 0, 0,
						  0, -size, 0, 0,  size, 0,
						  0, 0, -size, 0, 0,  size };

	GLfloat colors[] = { 1.0f, 0.0f, 0.196f, 1.0f, 1.0f, 0.0f, 0.196f, 1.0f,
						 0.196f, 1.0f, 0.0f, 1.0f, 0.196f, 1.0f, 0.0f, 1.0f,
						 0.0f, 0.564f, 1.0f, 1.0f, 0.0f, 0.564f, 1.0f, 1.0f };

	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Once);
	buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Once);
}