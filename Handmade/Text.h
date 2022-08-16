#pragma once
#include <map>
#include <ft2build.h>
#include <freetype\freetype.h>
#include "Buffer.h"
#include "Object.h"

class Text : public Object
{

public:

	static bool Initialize();
	static void Shutdown();

	Text(const std::string& tag,
		const std::string& filename = "", GLuint fontSize = 10U);
	virtual ~Text();

	bool Load(const std::string& tag,
		const std::string& filename, GLuint fontSize);
	void Unload(const std::string& tag = "");

	GLuint GetTotalWidth();
	GLuint GetFontSize() const;
	const std::string& GetString() const;

	void SetFont(const std::string& tag);
	void SetString(const std::string& string);
	void AppendString(const std::string& string);

	void IsFirstLetterCentered(bool flag);

	virtual void SetColor(const glm::vec4& color);
	virtual void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader);

private:

	const GLuint totalASCIICharacters = 128;

	struct Glyph
	{
		GLuint ID;       //OGL texture ID for each letter 
		GLint width;     //Width of the letter
		GLint height;    //Height of the letter 
		GLint bearingX;  //Distance from the y-axis origin
		GLint bearingY;  //Distance from the x-axis baseline 
		GLint advance;   //Offset to advance to next glyph
	};

	//Would love to call it 'Font' but that name is reserved
	typedef std::map<GLchar, Glyph> FontType;

	FontType font;
	Buffer buffer{ "Text_" + std::to_string(++totalObjects), 6, true };

	GLuint fontSize;
	GLuint totalWidth{ 0 };
	glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

	std::string string;
	bool isFirstLetterCentered{ false };

	static GLuint totalObjects;
	static std::string rootFolder;
	static FT_Library freetypeObject;
	static std::map<std::string, FontType> fonts;

};