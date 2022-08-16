#include <gtc\matrix_transform.hpp>      
#include "Input.h"
#include "Shader.h"
#include "Tile.h"

//======================================================================================================
Tile::Tile(const std::string& tag, const std::string& filename,
	GLfloat width, GLfloat height, GLuint spriteSheetCol, GLuint spriteSheetRow)
	: Object(tag), spriteSheetCol(spriteSheetCol), spriteSheetRow(spriteSheetRow),
	buffer(tag, spriteSheetCol* spriteSheetRow * 6, true)
{
	dimension = glm::vec2(width, height);

	//TODO - Find a better tag name
	texture.Load(filename, filename);

	auto offsetUV = 0U;
	auto offsetColor = 0U;
	auto offsetIndex = 0U;
	auto offsetVertex = 0U;

	const auto TOTAL_DIMENSION = spriteSheetCol * spriteSheetRow;
	const auto BYTES_PER_TILE_UV = static_cast<GLuint>(Buffer::ComponentSize::UV) *
		corners * sizeof(GLfloat);
	const auto BYTES_PER_TILE_COLOR = static_cast<GLuint>(Buffer::ComponentSize::RGBA) *
		corners * sizeof(GLfloat);
	const auto BYTES_PER_TILE_VERTEX = static_cast<GLuint>(Buffer::ComponentSize::XYZ) *
		corners * sizeof(GLfloat);
	const auto BYTES_PER_TILE_INDEX = static_cast<GLuint>(Buffer::ComponentSize::XYZ) *
		(corners - 1) * sizeof(GLuint);

	const auto TOTAL_BYTES_VBO_VERT = TOTAL_DIMENSION * BYTES_PER_TILE_VERTEX;
	const auto TOTAL_BYTES_VBO_COLOR = TOTAL_DIMENSION * BYTES_PER_TILE_COLOR;
	const auto TOTAL_BYTES_VBO_UV = TOTAL_DIMENSION * BYTES_PER_TILE_UV;
	const auto TOTAL_BYTES_EBO = TOTAL_DIMENSION * BYTES_PER_TILE_INDEX;

	buffer.LinkEBO();

	glm::vec2 halfDimension = dimension * 0.5f;

	//Calculate the width and height of each 'cel' relative to the entire texture 
	//This gives us a normalized dimension value for each 'cel' in the sprite sheet
	glm::vec2 celDimension(1.0f / spriteSheetCol, 1.0f / spriteSheetRow);

	GLuint count = 0;

	//Loop through the entire sprite sheet and generate 
	//vertices, color, and UV coordinates for each tile 
	for (GLuint row = 0; row < spriteSheetRow; row++)
	{
		for (GLuint col = 0; col < spriteSheetCol; col++)
		{
			GLfloat vertices[] = { -halfDimension.x,  halfDimension.y, 0.0f,
									halfDimension.x,  halfDimension.y, 0.0f,
									halfDimension.x, -halfDimension.y, 0.0f,
								   -halfDimension.x, -halfDimension.y, 0.0f };

			buffer.AppendVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
			offsetVertex += BYTES_PER_TILE_VERTEX;

			GLfloat colors[] = { color.r, color.g, color.b, color.a,
								 color.r, color.g, color.b, color.a,
								 color.r, color.g, color.b, color.a,
								 color.r, color.g, color.b, color.a };

			buffer.AppendVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), offsetColor);
			offsetColor += BYTES_PER_TILE_COLOR;

			//Take the desired 'cel' to 'cut out' and multiply it by the cel's dimension value
			//This gives us a normalized texture coordinate value that is our 'starting point' 
			glm::vec2 UVOrigin(col * celDimension.x, row * celDimension.y);

			GLfloat UVs[] = { UVOrigin.s,                  UVOrigin.t,
							  UVOrigin.s + celDimension.x, UVOrigin.t,
							  UVOrigin.s + celDimension.x, UVOrigin.t + celDimension.y,
							  UVOrigin.s,                  UVOrigin.t + celDimension.y };

			buffer.AppendVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), offsetUV);
			offsetUV += BYTES_PER_TILE_UV;

			GLuint indices[] = { 0 + (count * 4), 1 + (count * 4), 3 + (count * 4),
								 3 + (count * 4), 1 + (count * 4), 2 + (count * 4) };

			buffer.AppendEBO(indices, sizeof(indices), offsetIndex);
			offsetIndex += BYTES_PER_TILE_INDEX;
			count++;
		}
	}
}
//======================================================================================================
Tile::~Tile()
{
	buffer.Destroy(tag);
}
//======================================================================================================
bool Tile::IsAnimationDead() const
{
	return isAnimationDead;
}
//======================================================================================================
bool Tile::IsAnimationLooping() const
{
	return isAnimationLooping;
}
//======================================================================================================
void Tile::IsAnimated(bool flag)
{
	isAnimated = flag;
}
//======================================================================================================
void Tile::IsAnimationLooping(bool flag)
{
	isAnimationLooping = flag;
}
//======================================================================================================
void Tile::SetTileIndex(GLuint tileIndex)
{
	this->tileIndex = tileIndex;
}
//======================================================================================================
void Tile::SetTileIndex(GLuint column, GLuint row)
{
	//Because everything is zero-based, we must decrement
	tileIndex = ((row - 1) * spriteSheetCol) + column;
	tileIndex--;
}
//======================================================================================================
void Tile::SetAnimationVelocity(GLfloat velocity)
{
	animationVelocity = velocity;
}
//======================================================================================================
void Tile::SetDimension(const glm::vec3& dimension)
{
	this->dimension = dimension;
}
//======================================================================================================
void Tile::SetDimension(GLfloat width, GLfloat height)
{
	GLfloat halfWidth = width * 0.5f;
	GLfloat halfHeight = height * 0.5f;

	GLfloat vertices[] = { -halfWidth,  halfHeight, 0.0f,
							halfWidth,  halfHeight, 0.0f,
							halfWidth, -halfHeight, 0.0f,
						   -halfWidth, -halfHeight, 0.0f };

	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Tile::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	GLuint offsetColor = 0;

	for (int i = 0; i < (int)(spriteSheetCol * spriteSheetRow); i++)
	{
		GLfloat colors[] = { r, g, b, a,
							 r, g, b, a,
							 r, g, b, a,
							 r, g, b, a };

		buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);
		offsetColor += sizeof(colors);
	}
}
//======================================================================================================
void Tile::Render(Shader& shader)
{
	//Store size of each EBO partition for each tile
	const GLuint bytesPerTileIndex = verticesPerTile * sizeof(GLuint);

	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("textureIn"),
		Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);
	//   buffer.LinkVBO(shader.GetAttributeID("normalIn"),
		//Buffer::VBO::ColorBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);

	//This will render each individual 'cel' making it look like an animation
	if (isAnimated)
	{
		//Use fake 60FPS 'delta time'
		static GLfloat timeElapsed = 0.0f;
		timeElapsed += 0.16f;

		//If animation is set to cycle endlessly then set the kill and final flags
		//to false so that no other final checks are made and that the animation loops
		if (isAnimationLooping)
		{
			isAnimationDead = false;
			isAnimationLoopFinal = false;
		}

		//Otherwise if animation is set to cycle once and the last image
		//cel has been reached then flag this as the final animation loop
		else if (!isAnimationLooping &&
			tileIndex == (spriteSheetCol * spriteSheetRow - 1))
		{
			isAnimationLoopFinal = true;
		}

		//If this is the final animation, flag to kill entire animation
		//because even though the animation is marked final, a few more 
		//frames will be called with the last image cel set, so only
		//mark it dead when the first image cel comes around again
		if (isAnimationLoopFinal && tileIndex == 0)
		{
			isAnimationDead = true;
		}

		//We need to be able to pick the correct portion
		//of the EBO to use to render the correct tile
		//The index value is zero based and is a whole 
		//number value counting from top left and going right
		//and down the spritesheet, and is capable of 'wrapping'
		tileIndex = (GLuint)(timeElapsed * animationVelocity) %
			(spriteSheetCol * spriteSheetRow);
	}

	texture.Bind();
	buffer.Render(Buffer::RenderMode::Triangles,
		(tileIndex * bytesPerTileIndex), verticesPerTile);
	texture.Unbind();
}
//======================================================================================================
void Tile::SendToShader(Shader& shader)
{
	shader.SendData("isTextured", 1);
	shader.SendData("model", transform.GetMatrix());
}