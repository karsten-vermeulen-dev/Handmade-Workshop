#include "Input.h"
#include "Quad.h"

int Quad::totalQuads = 0;

//======================================================================================================
int Quad::GetTotalQuads()
{
	return totalQuads;
}
//======================================================================================================
Quad::Quad(const std::string& tag, GLfloat width, GLfloat height, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	: Object(tag), buffer(tag, 6, true)
{
	totalQuads++;
	color = glm::vec4(r, g, b, a);
	dimension = glm::vec2(width, height);

	glm::vec2 halfDimension = dimension * 0.5f;

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

	buffer.LinkEBO();
	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::NormalBuffer, normals, sizeof(normals), Buffer::Fill::Ongoing);
	buffer.FillEBO(indices, sizeof(indices), Buffer::Fill::Ongoing);
}
//======================================================================================================
Quad::~Quad()
{
	buffer.Destroy(tag);
	totalQuads--;
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

	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Quad::SetTextureScale(GLfloat width, GLfloat height)
{
	GLfloat UVs[] = { 0.0f, 0.0f,
					  width, 0.0f,
					  width, height,
					  0.0f, height };

	buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
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

	buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
	color = glm::vec4(r, g, b, a);
}
//======================================================================================================
void Quad::Render(Shader& shader)
{
	//TODO - Find a way to do this only once
	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("textureIn"),
		Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);
	// buffer.LinkVBO(shader.GetAttributeID("normalIn"),
		//Buffer::VBO::ColorBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);

	// normalMatrix = glm::inverse(glm::mat3( transform.GetMatrix()));
	//shader.SendData("normal",  normalMatrix);

	//Quick fix to allow child objects without parent objects (this avoids a crash)
	//TODO - What we require here is a proper parent/child linkage of objects
	if (parent)
	{
		shader.SendData("model", parent->GetTransform().GetMatrix() * transform.GetMatrix());
	}

	else
	{
		shader.SendData("model", transform.GetMatrix());
	}

	shader.SendData("isTextured", static_cast<GLuint>(isTextured));

	//shader.SendData("isText", false);
	shader.SendData("isTextured", false);

	buffer.Render(Buffer::RenderMode::Triangles);
}