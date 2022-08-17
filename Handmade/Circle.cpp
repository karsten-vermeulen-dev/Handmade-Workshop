#include "Circle.h"

int Circle::totalCircles = 0;

//======================================================================================================
int Circle::GetTotalCircles()
{
	return totalCircles;
}
//======================================================================================================
Circle::Circle(const std::string& tag,
	GLfloat radius, GLuint slices, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	: Object(tag), slices(slices), radius(radius), buffer(tag, slices + 2)
{
	totalCircles++;
	color = glm::vec4(r, g, b, a);

	auto offsetVertex = 0U;
	auto offsetColor = 0U;

	const auto bytesPerVertex = 3 * sizeof(GLfloat);
	const auto bytesPerColor = 4 * sizeof(GLfloat);

	//x amount of slices requires (x + 2) vertices
	//The starting centre vertex will be vertex #1 
	//and we add on (x + 1) surrounding vertices

	//We add enough bytes here to accomodate for all the slices
	//An extra vertex is added to accomodate the middle point
	const auto maxBytesVertexVBO = (slices + 2) * bytesPerVertex;
	const auto maxBytesColorVBO = (slices + 2) * bytesPerColor;

	buffer.FillVBO(Buffer::VBO::VertexBuffer,
		nullptr, maxBytesVertexVBO, Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer,
		nullptr, maxBytesColorVBO, Buffer::Fill::Ongoing);

	//The more slices, the more detailed the circle is drawn 
	GLfloat tempAngle = glm::radians(360.0f / static_cast<GLfloat>(slices));

	//Because we are using a triangle fan for rendering, we need a starting
	//vertex point. Set starting vertex to 0, which will be relative to where
	//the sphere is positioned in client code and set the starting color too
	glm::vec3 startVertex = glm::vec3(0.0f);
	glm::vec4 startColor = glm::vec4(r, g, b, a);

	buffer.AppendVBO(Buffer::VBO::VertexBuffer,
		&startVertex.x, bytesPerVertex, offsetVertex);
	buffer.AppendVBO(Buffer::VBO::ColorBuffer,
		&startColor.r, bytesPerColor, offsetColor);

	offsetVertex += bytesPerVertex;
	offsetColor += bytesPerColor;

	//Loop through the amount of slices passed and add an extra slice into 
	//the loop to close the sphere off. The loop will use sin and cos internally
	//to generate a vertex point in a circular method around the origin
	//The generated vertices and color data will be added to the sphere's buffer
	//and the offset will get updated each iteration and keep track of how many
	//bytes into each VBO we already are in order to fill the VBOs correctly

	for (GLuint i = 0; i < slices + 1; i++)
	{
		GLfloat vertices[] = { radius * cos(tempAngle * (i + 1)),      //X coord
							   radius * sin(tempAngle * (i + 1)),      //Y coord
							   0.0f };                                 //Z coord

		GLfloat colors[] = { r, g, b, a };

		buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += bytesPerVertex;
		offsetColor += bytesPerColor;
	}
}
//======================================================================================================
Circle::~Circle()
{
	buffer.Destroy(tag);
	totalCircles--;
}
//======================================================================================================
void Circle::SetRadius(GLfloat radius)
{
	GLuint offset = 0;
	GLfloat tempAngle = glm::radians(360.0f / static_cast<GLfloat>(slices));

	for (GLuint i = 0; i < slices + 1; i++)
	{
		GLfloat vertices[] = { radius * cos(tempAngle * (i + 1)),
							   radius * sin(tempAngle * (i + 1)),
							   0.0f };

		buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offset);
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
	buffer.AppendVBO(Buffer::VBO::ColorBuffer, &startColor.r, 16, offset);
	offset += sizeof(startColor);

	for (GLuint i = 0; i < slices + 1; i++)
	{
		GLfloat colors[] = { r, g, b, a };
		buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offset);
		offset += sizeof(colors);
	}

	color = glm::vec4(r, g, b, a);
}
//======================================================================================================
void Circle::Render(Shader& shader)
{
	auto matrix = GetFinalMatrix();
	shader.SendData("model", matrix);
	shader.SendData("isTextured", static_cast<GLuint>(isTextured));

	//TODO - Find a way to do this only once
	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

	//TODO - Add lighting back in when normals have been added
	if (isLit)
	{
		//buffer.LinkVBO(shader.GetAttributeID("normalIn"),
			//Buffer::VBO::NormalBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);

		//normalMatrix = glm::inverse(matrix);
		//shader.SendData("normal", normalMatrix, true);
		//material.SendToShader(shader);
	}

	buffer.Render(Buffer::RenderMode::TriangleFan);
}