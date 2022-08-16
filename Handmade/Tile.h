#pragma once
#include <string>
#include "GLAD/gl.h"
#include <glm.hpp>
#include "Buffer.h"
#include "Object.h"
#include "Texture.h"

class Tile : public Object
{

public:

	Tile(const std::string& tag, const std::string& filename,
		GLfloat width = 1.0f, GLfloat height = 1.0f,
		GLuint spriteSheetCol = 1, GLuint spriteSheetRow = 1);
	virtual ~Tile();

	bool IsAnimationDead() const;
	bool IsAnimationLooping() const;

	void IsAnimated(bool flag);
	void IsAnimationLooping(bool flag);

	void SetTileIndex(GLuint tileIndex);
	void SetTileIndex(GLuint column, GLuint row);
	void SetAnimationVelocity(GLfloat velocity);

	void SetDimension(const glm::vec3& dimension);
	void SetDimension(GLfloat width, GLfloat height);
	void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader);

private:

	bool isAnimated{ false };
	bool isAnimationDead{ false };
	bool isAnimationLooping{ false };
	bool isAnimationLoopFinal{ false };

	GLuint tileIndex{ 0 };
	GLuint spriteSheetCol{ 0 };
	GLuint spriteSheetRow{ 0 };
	GLfloat animationVelocity{ 0.1f };

	Buffer buffer;
	Texture texture;

	glm::vec4 color{ 1.0f };
	glm::vec2 dimension{ 0.0f };

	const GLuint corners{ 4 };
	const GLuint verticesPerTile{ 6 };

};