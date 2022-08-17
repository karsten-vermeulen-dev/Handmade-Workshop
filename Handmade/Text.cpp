#include <assert.h>
#include "Shader.h"
#include "Text.h"
#include "Utility.h"

GLuint Text::totalObjects = 0;
FT_Library Text::freetypeObject = nullptr;
std::string Text::rootFolder = "Assets/Fonts/";
std::map<std::string, Text::FontType> Text::fonts;

//================================================================================================
bool Text::Initialize()
{
	if (FT_Init_FreeType(&freetypeObject))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error initializing the FreeType font system.", Utility::Severity::Failure);
		return false;
	}

	return true;
}
//================================================================================================
void Text::Shutdown()
{
	//Unload();
	FT_Done_FreeType(freetypeObject);
}
//================================================================================================
bool Text::Load(const std::string& tag, const std::string& filename, GLuint fontSize)
{
	assert(fonts.find(tag) == fonts.end());
	FT_Face freetypeFace = nullptr;

	if (FT_New_Face(freetypeObject, (rootFolder + filename).c_str(), 0, &freetypeFace))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error loading font file \"" + (rootFolder + filename) + "\"\n\n"
			"Possible causes could be a corrupt or missing file. Another reason could be "
			"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);

		return false;
	}

	FT_Set_Pixel_Sizes(freetypeFace, fontSize, fontSize);

	//This is needed so that we don't follow the standard 4-byte alignment
	//because the freetype glyph textures only use 1 byte of color data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//FontType font;

	//Generate a glyph for all 128 ASCII characters
	for (int i = 0; i < (int)totalASCIICharacters; i++)
	{
		if (FT_Load_Char(freetypeFace, i, FT_LOAD_RENDER))
		{
			Utility::Log(Utility::Destination::WindowsMessageBox,
				"Error creating glyph", Utility::Severity::Warning);
			break;
		}

		//TODO - Find a way to move this to the lower levels
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RED,
			freetypeFace->glyph->bitmap.width,
			freetypeFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			freetypeFace->glyph->bitmap.buffer);

		glBindTexture(GL_TEXTURE_2D, 0);

		Glyph glyph = { textureID,
						static_cast<GLint>(freetypeFace->glyph->bitmap.width),
						static_cast<GLint>(freetypeFace->glyph->bitmap.rows),
						static_cast<GLint>(freetypeFace->glyph->bitmap_left),
						static_cast<GLint>(freetypeFace->glyph->bitmap_top),
						static_cast<GLint>(freetypeFace->glyph->advance.x) };

		font[i] = glyph;
	}

	FT_Done_Face(freetypeFace);
	fonts[tag] = font;
	return true;
}
//================================================================================================
void Text::Unload(const std::string& tag)
{
	if (!tag.empty())
	{
		auto it = fonts.find(tag);
		assert(it != fonts.end());

		for (const auto& glyph : it->second)
		{
			glDeleteTextures(1, &glyph.second.ID);
		}

		fonts.erase(it);
	}

	else
	{
		for (auto& font : fonts)
		{
			for (const auto& glyph : font.second)
			{
				glDeleteTextures(1, &glyph.second.ID);
			}
		}

		fonts.clear();
	}
}
//================================================================================================
Text::Text(const std::string& tag, const std::string& filename, GLuint fontSize)
	: Object(tag), fontSize(fontSize)
{
	buffer.LinkEBO();

	if (!filename.empty())
	{
		Load(tag, filename, fontSize);
		SetFont(tag);
	}

	else if (!tag.empty())
	{
		SetFont(tag);
	}
}
//================================================================================================
Text::~Text()
{
	font.clear();
	buffer.Destroy(buffer.GetTag());
}
//================================================================================================
GLuint Text::GetFontSize() const
{
	return fontSize;
}
//================================================================================================
GLuint Text::GetTotalWidth()
{
	totalWidth = 0;

	for (const auto& character : string)
	{
		totalWidth += font[character].advance / 64;
	}

	return totalWidth;
}
//================================================================================================
const std::string& Text::GetString() const
{
	return string;
}
//================================================================================================
void Text::SetFont(const std::string& tag)
{
	auto it = fonts.find(tag);
	assert(it != fonts.end());
	font = it->second;
	this->tag = tag;
}
//================================================================================================
void Text::SetString(const std::string& string)
{
	this->string = string;
}
//================================================================================================
void Text::AppendString(const std::string& string)
{
	this->string += string;
}
//================================================================================================
void Text::SetColor(const glm::vec4& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}
//================================================================================================
void Text::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	color = glm::vec4(r, g, b, a);
}
//================================================================================================
void Text::IsFirstLetterCentered(bool flag)
{
	isFirstLetterCentered = flag;
}
//================================================================================================
void Text::Render(Shader& shader)
{
	assert(!tag.empty());

	glm::vec2 textOrigin = glm::vec2(0.0f);

	for (const auto& character : string)
	{
		Glyph glyph = font[character];

		auto halfWidth = glyph.width * 0.5f;
		auto halfBearingY = glyph.bearingY * 0.5f;

		if (isFirstLetterCentered)
		{
			GLfloat vertices[] = { textOrigin.x - halfWidth, textOrigin.y + halfBearingY, 0.0f,
				textOrigin.x + halfWidth, textOrigin.y + halfBearingY, 0.0f,
				textOrigin.x + halfWidth, textOrigin.y + halfBearingY - (GLfloat)glyph.height, 0.0f,
				textOrigin.x - halfWidth, textOrigin.y + halfBearingY - (GLfloat)glyph.height, 0.0f };
			buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
		}

		else
		{
			GLfloat vertices[] = { textOrigin.x + (GLfloat)glyph.bearingX, (GLfloat)glyph.bearingY, 0.0f,
				textOrigin.x + (GLfloat)glyph.bearingX + (GLfloat)glyph.width, (GLfloat)glyph.bearingY, 0.0f,
				textOrigin.x + (GLfloat)glyph.bearingX + (GLfloat)glyph.width, (GLfloat)glyph.bearingY - (GLfloat)glyph.height, 0.0f,
				textOrigin.x + (GLfloat)glyph.bearingX, (GLfloat)glyph.bearingY - (GLfloat)glyph.height, 0.0f };
			buffer.FillVBO(Buffer::VBO::VertexBuffer, vertices, sizeof(vertices), Buffer::Fill::Ongoing);
		}

		GLfloat colors[] = { color.r,    color.g,    color.b,    color.a,
			   color.r,    color.g,    color.b,    color.a,
			   color.r,    color.g,    color.b,    color.a,
			   color.r,    color.g,    color.b,    color.a };

		GLfloat UVs[] = { 0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f };

		GLuint indices[] = { 0, 1, 3,
			3, 1, 2 };

		buffer.FillEBO(indices, sizeof(indices));
		buffer.FillVBO(Buffer::VBO::TextureBuffer, UVs, sizeof(UVs), Buffer::Fill::Ongoing);
		buffer.FillVBO(Buffer::VBO::ColorBuffer, colors, sizeof(colors), Buffer::Fill::Ongoing);

		//TODO - Find a way to do this only once
		buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
			Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
		buffer.LinkVBO(shader.GetAttributeID("colorIn"),
			Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);
		buffer.LinkVBO(shader.GetAttributeID("textureIn"),
			Buffer::VBO::TextureBuffer, Buffer::ComponentSize::UV, Buffer::DataType::FloatData);

		glBindTexture(GL_TEXTURE_2D, glyph.ID);

		buffer.Render(Buffer::RenderMode::Triangles);

		glBindTexture(GL_TEXTURE_2D, 0);

		//We have to divide by 64 because the value is a product of 64
		textOrigin.x += (glyph.advance) / 64.0f;
	}
}
//================================================================================================
void Text::SendToShader(Shader& shader)
{
	shader.SendData("model", transform.GetMatrix());
}