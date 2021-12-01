#include "House.h"

House::House(GLfloat width, GLfloat height, GLfloat depth,
	GLfloat r, GLfloat g, GLfloat b, GLfloat a) : m_buffer("House", 66, true)
{
	m_dimension = glm::vec3(width, height, depth);

	glm::vec3 halfDimension = m_dimension * 0.5f;
	GLuint vertices[] = {
		-halfDimension.x,-halfDimension.y,halfDimension.z,
		-halfDimension.x,halfDimension.y,halfDimension.z,
		halfDimension.x,halfDimension.y,halfDimension.z,
		halfDimension.x,-halfDimension.y,halfDimension.z,
		-halfDimension.x,-halfDimension.y,-halfDimension.z,
	};
	GLuint indices[] = { // front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		6, 8, 5, //top triangle
		8, 1, 5, //left triangle
		2, 8, 6, //right triangle
		8, 6, 2,
		1, 8, 2, //bottom triangle
		8, 2, 1,
		// bottom face
		4, 0, 3,
		4, 3, 7 };

}

House::~House()
{
}

void House::SetTextureScale(GLfloat width, GLfloat height)
{
}

void House::SetColor(const glm::vec4& color)
{
}

void House::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
}

void House::SetDimension(const glm::vec3& dimension)
{
}

void House::SetDimension(GLfloat width, GLfloat height, GLfloat depth)
{
}

void House::Render(Shader& shader)
{
}
