#include "Circle.h"

int Circle::s_totalCircles = 0;
//======================================================================================================
int Circle::GetTotalCircles()
{
	return s_totalCircles;
}
//======================================================================================================
Circle::Circle(const std::string& tag,
	GLfloat radius, GLuint slices, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	: Object(tag), m_slices(slices), m_radius(radius), m_buffer(tag, slices + 2)
{
	s_totalCircles++;
	m_color = glm::vec4(r, g, b, a);

	auto offsetVertex = 0U;
	auto offsetColor = 0U;

	const auto BYTES_PER_VERTEX = 3 * sizeof(GLfloat);
	const auto BYTES_PER_COLOR = 4 * sizeof(GLfloat);

	//x amount of slices requires (x + 2) vertices
	//The starting centre vertex will be vertex #1 
	//and we add on (x + 1) surrounding vertices

	//We add enough bytes here to accomodate for all the slices
	//An extra vertex is added to accomodate the middle point
	const auto TOTAL_BYTES_VERTEX_VBO = (m_slices + 2) * BYTES_PER_VERTEX;
	const auto TOTAL_BYTES_COLOR_VBO = (m_slices + 2) * BYTES_PER_COLOR;

	m_buffer.FillVBO(Buffer::VBO::VertexBuffer,
		nullptr, TOTAL_BYTES_VERTEX_VBO, Buffer::Fill::Ongoing);
	m_buffer.FillVBO(Buffer::VBO::ColorBuffer,
		nullptr, TOTAL_BYTES_COLOR_VBO, Buffer::Fill::Ongoing);

	//The more slices, the more detailed the circle is drawn 
	GLfloat tempAngle = glm::radians(360.0f / static_cast<GLfloat>(m_slices));

	//Because we are using a triangle fan for rendering, we need a starting
	//vertex point. Set starting vertex to 0, which will be relative to where
	//the sphere is positioned in client code and set the starting color too
	glm::vec3 startVertex = glm::vec3(0.0f);
	glm::vec4 startColor = glm::vec4(r, g, b, a);

	m_buffer.AppendVBO(Buffer::VBO::VertexBuffer,
		&startVertex.x, BYTES_PER_VERTEX, offsetVertex);
	m_buffer.AppendVBO(Buffer::VBO::ColorBuffer,
		&startColor.r, BYTES_PER_COLOR, offsetColor);

	offsetVertex += BYTES_PER_VERTEX;
	offsetColor += BYTES_PER_COLOR;

	//Loop through the amount of slices passed and add an extra slice into 
	//the loop to close the sphere off. The loop will use sin and cos internally
	//to generate a vertex point in a circular method around the origin
	//The generated vertices and color data will be added to the sphere's buffer
	//and the offset will get updated each iteration and keep track of how many
	//bytes into each VBO we already are in order to fill the VBOs correctly

	for (GLuint i = 0; i < m_slices + 1; i++)
	{
		GLfloat vertices[] = { m_radius * cos(tempAngle * (i + 1)),      //X coord
							   m_radius * sin(tempAngle * (i + 1)),      //Y coord
							   0.0f };  //Z coord

		GLfloat colors[] = { r, g, b, a };

		m_buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		m_buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += BYTES_PER_VERTEX;
		offsetColor += BYTES_PER_COLOR;
	}
}
//======================================================================================================
Circle::~Circle()
{
	m_buffer.Destroy(m_tag);
	s_totalCircles--;
}
//======================================================================================================
void Circle::SetRadius(GLfloat radius)
{
	GLuint offset = 0;
	GLfloat tempAngle = glm::radians(360.0f / static_cast<GLfloat>(m_slices));

	for (GLuint i = 0; i < m_slices + 1; i++)
	{
		GLfloat vertices[] = { radius * cos(tempAngle * (i + 1)),
							   radius * sin(tempAngle * (i + 1)),
							   0.0f };

		m_buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offset);
		offset += sizeof(vertices);
	}
}
//======================================================================================================
void Circle::SetColor(const glm::vec4& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}
//======================================================================================================
void Circle::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLuint offset = 0;
	glm::vec4 startColor = glm::vec4(r, g, b, a);
	m_buffer.AppendVBO(Buffer::VBO::ColorBuffer, &startColor.r, 16, offset);

	for (GLuint i = 0; i < m_slices + 1; i++)
	{
		GLfloat colors[] = { r, g, b, a };
		m_buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offset);
		offset += sizeof(colors);
	}

	m_color = glm::vec4(r, g, b, a);
}
//======================================================================================================
void Circle::Render(Shader& shader)
{
	//TODO - Find a way to do this only once
	m_buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	m_buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

	//Quick fix to allow child objects without parent objects (this avoids a crash)
	//TODO - What we require here is a proper parent/child linkage of objects
	if (m_parent)
	{
		shader.SendData("model", m_parent->GetTransform().GetMatrix() * m_transform.GetMatrix());
	}

	else
	{
		shader.SendData("model", m_transform.GetMatrix());
	}

	m_buffer.Render(Buffer::RenderMode::TriangleFan);
}