#include "Cuboid.h"
#include "Input.h"

int Cuboid::totalCuboids = 0;

//======================================================================================================
int Cuboid::GetTotalCuboids()
{
	return totalCuboids;
}
//======================================================================================================
Cuboid::Cuboid(const std::string& tag, GLfloat width, GLfloat height, GLfloat depth,
	GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	: Object(tag), buffer(tag, 36, true)
{
	totalCuboids++;
	color = glm::vec4(r, g, b, a);
	dimension = glm::vec3(width, height, depth);

	glm::vec3 halfDimension = dimension * 0.5f;

	GLfloat vertices[] = { -halfDimension.x,  halfDimension.y,  halfDimension.z,
							halfDimension.x,  halfDimension.y,  halfDimension.z,
							halfDimension.x, -halfDimension.y,  halfDimension.z,
						   -halfDimension.x, -halfDimension.y,  halfDimension.z,       //Front face

							halfDimension.x,  halfDimension.y, -halfDimension.z,
						   -halfDimension.x,  halfDimension.y, -halfDimension.z,
						   -halfDimension.x, -halfDimension.y, -halfDimension.z,
							halfDimension.x, -halfDimension.y, -halfDimension.z,       //Back face

						   -halfDimension.x,  halfDimension.y, -halfDimension.z,
						   -halfDimension.x,  halfDimension.y,  halfDimension.z,
						   -halfDimension.x, -halfDimension.y,  halfDimension.z,
						   -halfDimension.x, -halfDimension.y, -halfDimension.z,       //Left face

							halfDimension.x,  halfDimension.y,  halfDimension.z,
							halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x, -halfDimension.y, -halfDimension.z,
							halfDimension.x, -halfDimension.y,  halfDimension.z,       //Right face

						   -halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x,  halfDimension.y,  halfDimension.z,
						   -halfDimension.x,  halfDimension.y,  halfDimension.z,       //Top face

						   -halfDimension.x, -halfDimension.y,  halfDimension.z,
							halfDimension.x, -halfDimension.y,  halfDimension.z,
							halfDimension.x, -halfDimension.y, -halfDimension.z,
						   -halfDimension.x, -halfDimension.y, -halfDimension.z };    //Bottom face


	GLfloat colors[] = { r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,     //Front face 

						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,     //Back face

						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,     //Left face

						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,     //Right face

						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,     //Top face

						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a };   //Bottom face


	GLfloat UVs[] = { 0.0f, 1.0f, 1.0f, 1.0f,
					  1.0f, 0.0f, 0.0f, 0.0f,      //Front face 

					  0.0f, 1.0f, 1.0f, 1.0f,
					  1.0f, 0.0f, 0.0f, 0.0f,	   //Back face

					  0.0f, 1.0f, 1.0f, 1.0f,
					  1.0f, 0.0f, 0.0f, 0.0f,	   //Left face

					  0.0f, 1.0f, 1.0f, 1.0f,
					  1.0f, 0.0f, 0.0f, 0.0f,	   //Right face

					  0.0f, 1.0f, 1.0f, 1.0f,
					  1.0f, 0.0f, 0.0f, 0.0f,	   //Top face

					  0.0f, 1.0f, 1.0f, 1.0f,
					  1.0f, 0.0f, 0.0f, 0.0f };    //Bottom face


	GLfloat normals[] = { 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
						  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,      //Front face 

						   0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
						   0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,	    //Back face

						  -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
						  -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,	    //Left face

						   1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
						   1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,	    //Right face

						   0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
						   0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,	    //Top face

						   0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
						   0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f };   //Bottom face


	GLuint indices[] = { 0,  1,  3,  3,  1,  2,      //Front face
						 4,  5,  7,  7,  5,  6,      //Back face 
						 8,  9, 11, 11,  9, 10,      //Left face
						12, 13, 15, 15, 13, 14,      //Right face
						16, 17, 19, 19, 17, 18,      //Top face
						20, 21, 23, 23, 21, 22 };    //Bottom face

	//TODO - Find a way to only create one single 
	//buffer to be shared amongst subsequent cuboids

	buffer.LinkEBO();
	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::NormalBuffer, normals, sizeof(normals), Buffer::Fill::Ongoing);
	buffer.FillEBO(indices, sizeof(indices), Buffer::Fill::Ongoing);
}
//======================================================================================================
Cuboid::~Cuboid()
{
	buffer.Destroy(tag);
	totalCuboids--;
}
//======================================================================================================
void Cuboid::SetTextureScale(GLfloat width, GLfloat height)
{
	GLfloat UVs[] = { 0.0f,  0.0f,   width, 0.0f,
					  width, height, 0.0f,  height, 	//front face
					  width, 0.0f,   0.0f,  0.0f,
					  0.0f,  height, width, height, 	//back face	
					  0.0f,  0.0f,   width, 0.0f,
					  width, height, 0.0f,  height,		//left face 			 
					  width, 0.0f,   0.0f,  0.0f,
					  0.0f,  height, width, height,		//right face 			 
					  0.0f,  0.0f,   width, 0.0f,
					  width, height, 0.0f,  height,		//top face 			 
					  0.0f,  height, width, height,
					  width, 0.0f,   0.0f,  0.0f };   	//bottom face

	buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Cuboid::SetColor(const glm::vec4& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}
//======================================================================================================
void Cuboid::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLfloat colors[] = { r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,       //front face 
						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,       //back face 
						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,       //left face 
						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,       //right face 
						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a,       //top face 
						 r, g, b, a, r, g, b, a,
						 r, g, b, a, r, g, b, a };     //bottom face 

	buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
	color = glm::vec4(r, g, b, a);
}
//======================================================================================================
void Cuboid::SetDimension(const glm::vec3& dimension)
{
	SetDimension(dimension.x, dimension.y, dimension.z);
}
//======================================================================================================
void Cuboid::SetDimension(GLfloat width, GLfloat height, GLfloat depth)
{
	dimension = glm::vec3(width, height, depth);
	glm::vec3 halfDimension = dimension * 0.5f;

	GLfloat vertices[] = { -halfDimension.x,  halfDimension.y,  halfDimension.z,
							halfDimension.x,  halfDimension.y,  halfDimension.z,
							halfDimension.x, -halfDimension.y,  halfDimension.z,
						   -halfDimension.x, -halfDimension.y,  halfDimension.z,       //Front face

						   -halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x, -halfDimension.y, -halfDimension.z,
						   -halfDimension.x, -halfDimension.y, -halfDimension.z,       //Back face

						   -halfDimension.x,  halfDimension.y, -halfDimension.z,
						   -halfDimension.x,  halfDimension.y,  halfDimension.z,
						   -halfDimension.x, -halfDimension.y,  halfDimension.z,
						   -halfDimension.x, -halfDimension.y, -halfDimension.z,       //Left face

							halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x,  halfDimension.y,  halfDimension.z,
							halfDimension.x, -halfDimension.y,  halfDimension.z,
							halfDimension.x, -halfDimension.y, -halfDimension.z,       //Right face

						   -halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x,  halfDimension.y, -halfDimension.z,
							halfDimension.x,  halfDimension.y,  halfDimension.z,
						   -halfDimension.x,  halfDimension.y,  halfDimension.z,       //Top face

						   -halfDimension.x, -halfDimension.y, -halfDimension.z,
							halfDimension.x, -halfDimension.y, -halfDimension.z,
							halfDimension.x, -halfDimension.y,  halfDimension.z,
						   -halfDimension.x, -halfDimension.y,  halfDimension.z };    //Bottom face

	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Cuboid::Render(Shader& shader)
{
	auto matrix = GetFinalMatrix();
	shader.SendData("model", matrix);
	shader.SendData("isTextured", static_cast<GLuint>(isTextured));

	//TODO - Find a way to do this only once
	//TODO - Find a way to individually set shader attributes based on different shaders
	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("textureIn"),
		Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);
	
	if (isLit)
	{
		buffer.LinkVBO(shader.GetAttributeID("normalIn"),
			Buffer::VBO::NormalBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);

		normalMatrix = glm::inverse(matrix);
		shader.SendData("normal", normalMatrix, true);
		material.SendToShader(shader);
	}

	buffer.Render(Buffer::RenderMode::Triangles);
}