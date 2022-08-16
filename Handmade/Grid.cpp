#include <algorithm>
#include "Grid.h"
#include "Input.h"
#include "Shader.h"

//Note:
//We multiply by 2 to represent each line's vertex 
//and add 4 for the two extra lines to close the grid
// buffer("Grid", (size * 4 * 2) + 4) 

//======================================================================================================
Grid::Grid(const std::string& tag, bool hasSpaceForAxes, GLint size, GLfloat lineWidth)
	: Object(tag), size(size), buffer(tag, (size * 4 * 2) + 4), //TODO - Remove magic numbers
	lineWidth(lineWidth), hasSpaceForAxes(hasSpaceForAxes)  //QUADRANTS = 4
{
	Create();
}
//======================================================================================================
Grid::~Grid()
{
	buffer.Destroy(tag);
}
//======================================================================================================
void Grid::SetSize(GLint size)
{
	size = size;
	size = std::max(size, 1);
	Create();
}
//======================================================================================================
void Grid::SetLineWidth(GLfloat lineWidth)
{
	this->lineWidth = lineWidth;
}
//======================================================================================================
void Grid::SetColor(const glm::vec4& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}
//======================================================================================================
void Grid::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLuint offset = 0;

	for (auto i = 0; i < size * (GLint)quadrants; i++)
	{
		GLfloat colors[] = { r, g, b, a, r, g, b, a };
		buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offset);
		offset += sizeof(colors);
	}
	color = glm::vec4(r, g, b, a);
}
//======================================================================================================
void Grid::Render(Shader& shader)
{
	Buffer::SetLineWidth(lineWidth);

	//TODO - Find a way to do this only once
	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::IntData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

	shader.SendData("isTextured", false);
	shader.SendData("model", GetFinalMatrix());

	buffer.Render(Buffer::RenderMode::Lines);
}
//======================================================================================================
void Grid::Create()
{
	//We need to add an extra line to close the grid 
	//Each line has two vertex and two color groups
	const GLuint bytesPerVertex =
		static_cast<GLuint>(Buffer::ComponentSize::XYZ) * sizeof(GLint);
	const GLuint bytesPerColor =
		static_cast<GLuint>(Buffer::ComponentSize::RGBA) * sizeof(GLfloat);

	const GLuint maxBytesVertexVBO = (size + 1) * quadrants * bytesPerVertex * 2;
	const GLuint maxBytesColorVBO = (size + 1) * quadrants * bytesPerColor * 2;

	//TODO - Fix this
	//We don't want to create new buffer 
	//objects everytime the grid is resized
	/*if (! buffer.GetTag().empty())
	{
		 buffer.Destroy();
	}*/

	buffer.FillVBO(Buffer::VBO::VertexBuffer,
		(GLfloat*)nullptr, maxBytesVertexVBO, Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer,
		(GLfloat*)nullptr, maxBytesColorVBO, Buffer::Fill::Ongoing);

	GLuint offsetColor = 0;
	GLuint offsetVertex = 0;

	//==================================
	//Negative vertical lines
	//==================================

	for (int i = 0; hasSpaceForAxes ? i < size : i <= size; i++)
	{
		GLint vertices[] = { -size + i, 0,   size,      //first vertex
							 -size + i, 0, -size };     //second vertex

		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 0.5f,
							 0.5f, 0.5f, 0.5f, 0.5f };

		buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += bytesPerVertex * 2;
		offsetColor += bytesPerColor * 2;
	}

	//==================================
	//Positive vertical lines
	//==================================

	for (int i = 1; i < size + 1; i++)
	{
		GLint vertices[] = { 0 + i, 0,   size,      //first vertex
							 0 + i, 0, -size };     //second vertex

		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 0.5f,
							 0.5f, 0.5f, 0.5f, 0.5f };

		buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += bytesPerVertex * 2;
		offsetColor += bytesPerColor * 2;
	}

	//==================================
	//Negative horizontal lines
	//==================================

	for (int i = 0; hasSpaceForAxes ? i < size : i <= size; i++)
	{
		GLint vertices[] = { -size, 0, -size + i,        //first vertex
							   size, 0, -size + i };     //second vertex

		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 0.5f,
							 0.5f, 0.5f, 0.5f, 0.5f };

		buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += bytesPerVertex * 2;
		offsetColor += bytesPerColor * 2;
	}

	//==================================
	//Positive horizontal lines
	//==================================

	for (int i = 1; i < size + 1; i++)
	{
		GLint vertices[] = { -size, 0, 0 + i,		//first vertex
							   size, 0, 0 + i };	//second vertex

		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 0.5f,
							 0.5f, 0.5f, 0.5f, 0.5f };

		buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += bytesPerVertex * 2;
		offsetColor += bytesPerColor * 2;
	}
}