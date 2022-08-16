#include "Line.h"

//======================================================================================================
Line::Line(const std::string& tag, GLfloat lineWidth, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	: Object(tag), buffer(tag, 2), lineWidth(lineWidth)
{
	GLfloat colors[] = { r, g, b, a, r, g, b, a };
	GLfloat vertices[] = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
}
//======================================================================================================
Line::~Line()
{
	buffer.Destroy(tag);
}
//======================================================================================================
void Line::SetEndPoints(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
	GLfloat vertices[] = { x1, y1, z1, x2, y2, z2 };
	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Line::SetColor(const glm::vec4& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}
//======================================================================================================
void Line::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLfloat colors[] = { r, g, b, a,
						 r, g, b, a,
						 r, g, b, a,
						 r, g, b, a };

	buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
	color = glm::vec4(r, g, b, a);
}
//======================================================================================================
void Line::Render(Shader& shader)
{
	//TODO - Find a way to do this only once
	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

	Buffer::SetLineWidth(lineWidth);
	buffer.Render(Buffer::RenderMode::Lines);
}