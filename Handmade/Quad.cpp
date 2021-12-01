#include "Input.h"
#include "Quad.h"

//======================================================================================================
Quad::Quad(GLfloat width, GLfloat height, GLfloat r, GLfloat g, GLfloat b, GLfloat a) 
	: m_buffer("Quad_", 6, true)
{
	m_dimension = glm::vec2(width, height);

	glm::vec2 halfDimension = m_dimension * 0.5f;

	GLfloat vertices[] = { -halfDimension.x,  halfDimension.y, 0.0f,
							halfDimension.x,  halfDimension.y, 0.0f,
							halfDimension.x, -halfDimension.y, 0.0f,
						   -halfDimension.x, -halfDimension.y, 0.0f };

	GLfloat colors[] = { r, g, b, a,
						 r, g, b, a,
						 r, g, b, a,
						 r, g, b, a };

	GLfloat UVs[] = { 0.0f, 1.0f,
					  1.0f, 1.0f,
					  1.0f, 0.0f,
					  0.0f, 0.0f };

	GLfloat normals[] = { 0.0f, 0.0f, 1.0f,
						  0.0f, 0.0f, 1.0f,
						  0.0f, 0.0f, 1.0f,
						  0.0f, 0.0f, 1.0f };

	GLuint indices[] = { 0, 1, 3,
						 3, 1, 2 };

	//TODO - Find a way to only create one single 
	//buffer to be shared amongst subsequent quads
	//m_buffer.Create("Quad", 6, true);

	m_buffer.LinkEBO();
	m_buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
	m_buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
	m_buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
	m_buffer.FillVBO(Buffer::VBO::NormalBuffer, normals, sizeof(normals), Buffer::Fill::Ongoing);
	m_buffer.FillEBO(indices, sizeof(indices), Buffer::Fill::Ongoing);

	m_linkOnce = false;
}
//======================================================================================================
Quad::~Quad()
{
	m_buffer.Destroy("Quad_");
}
//======================================================================================================
void Quad::SetDimension(const glm::vec2& dimension)
{
	SetDimension(dimension.x, dimension.y);
}
//======================================================================================================
void Quad::SetDimension(GLfloat width, GLfloat height)
{
	glm::vec2 halfDimension = glm::vec2(width * 0.5f, height * 0.5f);

	GLfloat vertices[] = { -halfDimension.x,  halfDimension.y, 0.0f,
							halfDimension.x,  halfDimension.y, 0.0f,
							halfDimension.x, -halfDimension.y, 0.0f,
						   -halfDimension.x, -halfDimension.y, 0.0f };

	m_buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Quad::SetTextureScale(GLfloat width, GLfloat height)
{
	GLfloat UVs[] = { 0.0f, 0.0f,
					  width, 0.0f,
					  width, height,
					  0.0f, height };

	m_buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Quad::SetColor(const glm::vec4& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}
//======================================================================================================
void Quad::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLfloat colors[] = { r, g, b, a,
						 r, g, b, a,
						 r, g, b, a,
						 r, g, b, a };

	m_buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Quad::Render(Shader& shader)
{
	//TODO - Find a way to do this only once
	if (!m_linkOnce)
	{
		m_buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
			Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
		m_buffer.LinkVBO(shader.GetAttributeID("colorIn"),
			Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
		m_buffer.LinkVBO(shader.GetAttributeID("textureIn"),
			Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);
		//m_buffer.LinkVBO(shader.GetAttributeID("normalIn"),
			//Buffer::VBO::ColorBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
		m_linkOnce = true;
	}
	//m_normalMatrix = glm::inverse(glm::mat3(m_transform.GetMatrix()));
	//shader.SendData("normal", m_normalMatrix);

	shader.SendData("model", m_transform.GetMatrix());
	shader.SendData("isTextured", static_cast<GLuint>(m_isTextured));

	//shader.SendData("isText", false);
	shader.SendData("isTextured", false);

	m_buffer.Render(Buffer::RenderMode::Triangles);
}