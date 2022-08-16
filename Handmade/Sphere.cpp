#include "Shader.h"
#include "Sphere.h"

int Sphere::totalSpheres = 0;

//======================================================================================================
int Sphere::GetTotalSpheres()
{
	return totalSpheres;
}
//======================================================================================================
Sphere::Sphere(const std::string& tag, GLfloat radius, GLuint segments, GLuint slices,
	GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	: Object(tag), slices(slices), radius(radius), segments(segments),
	buffer(tag, segments* (slices - 1) * 6, true)
{
	totalSpheres++;
	color = glm::vec4(r, g, b, a);

	GLuint offsetVertex = 0;
	GLuint offsetColor = 0;
	GLuint offsetIndex = 0;

	const auto bytesPerVertex =
		static_cast<GLuint>(Buffer::ComponentSize::XYZ) * sizeof(GLfloat);
	const auto bytesPerColor =
		static_cast<GLuint>(Buffer::ComponentSize::RGBA) * sizeof(GLfloat);
	const auto bytesPerTriangle =
		static_cast<GLuint>(Buffer::ComponentSize::XYZ) * sizeof(GLuint);

	const auto maxBytesEBO = (slices - 1) * segments * bytesPerTriangle * 2;
	const auto maxBytesVertexVBO = (slices + 1) * (segments + 1) * bytesPerVertex;
	const auto maxBytesColorVBO = (slices + 1) * (segments + 1) * bytesPerColor;

	//We multiply by 6 because each slice creates 1 quad (2 triangles)
	buffer.LinkEBO();
	buffer.FillEBO(nullptr, maxBytesEBO, Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::VertexBuffer,
		nullptr, maxBytesVertexVBO, Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer,
		nullptr, maxBytesColorVBO, Buffer::Fill::Ongoing);

	//Use this as a reference for sphere equation:
	//http://mathworld.wolfram.com/Sphere.html

	//We first need to calculate the longitude and latitude angle
	GLfloat angleLongitude = glm::radians(360.0f / static_cast<GLfloat>(segments));
	GLfloat angleLatitude = glm::radians(180.0f / static_cast<GLfloat>(slices));

	for (GLuint i = 0; i < segments + 1; i++)
	{
		for (GLuint j = 0; j <= slices; j++)
		{
			GLfloat vertices[] = { radius * cos(angleLongitude * i) * sin(angleLatitude * j),  //X 
									radius * sin(angleLongitude * i) * sin(angleLatitude * j),  //Y 
									radius * cos(angleLatitude * j) };                          //Z 

			GLfloat colors[] = { color.r,  color.g,  color.b,  color.a };

			buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
			buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);

			offsetVertex += bytesPerVertex;
			offsetColor += bytesPerColor;
		}
	}

	for (GLuint i = 0; i < segments; i++)
	{
		for (GLuint j = 0; j < slices - 1; j++)
		{
			GLuint indices[] = { (i * (slices + 1)) + j,                         //triangle 1 - X
								 (i * (slices + 1)) + j + 1,					 //triangle 1 - Y
								 (i * (slices + 1)) + j + slices + 2,            //triangle 1 - Z

								 (i * (slices + 1)) + ((slices * 2) + 1) - j,    //triangle 2 - X
								 (i * (slices + 1)) + ((slices * 2)) - j,		 //triangle 2 - Y
								 (i * (slices + 1)) + ((slices - 1)) - j };	     //triangle 2 - Z

			buffer.AppendEBO(indices, sizeof(indices), offsetIndex);
			offsetIndex += bytesPerTriangle * 2;
		}
	}
}
//======================================================================================================
Sphere::~Sphere()
{
	buffer.Destroy(tag);
	totalSpheres--;
}
//======================================================================================================
void Sphere::SetRadius(GLfloat radius)
{
	GLuint offset = 0;
	GLfloat angleLongitude = glm::radians(360.0f / static_cast<GLfloat>(segments));
	GLfloat angleLatitude = glm::radians(180.0f / static_cast<GLfloat>(slices));

	for (GLuint i = 0; i < segments + 1; i++)
	{
		for (GLuint j = 0; j <= slices; j++)
		{
			GLfloat vertices[] = { radius * cos(angleLongitude * i) * sin(angleLatitude * j),   //X 
								   radius * sin(angleLongitude * i) * sin(angleLatitude * j),   //Y 
								   radius * cos(angleLatitude * j) };                           //Z 

			buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offset);
			offset += sizeof(vertices);
		}
	}
}
//======================================================================================================
void Sphere::SetColor(const glm::vec4& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}
//======================================================================================================
void Sphere::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLuint offset = 0;

	for (GLuint i = 0; i < segments + 1; i++)
	{
		for (GLuint j = 0; j <= slices; j++)
		{
			GLfloat colors[] = { r, g, b, a };
			buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offset);
			offset += sizeof(colors);
		}
	}
	color = glm::vec4(r, g, b, a);
}
//======================================================================================================
void Sphere::Render(Shader& shader)
{
	//TODO - Find a way to do this only once
	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

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

	buffer.Render(Buffer::RenderMode::Triangles);
}