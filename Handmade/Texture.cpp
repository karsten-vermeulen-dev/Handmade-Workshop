#include <assert.h>
#include <iostream>
#include <SDL_image.h>
#include "Texture.h"
#include "Utility.h"

std::map<std::string, Texture> Texture::s_textures;
std::string Texture::s_rootFolder = "Assets/Textures/";

//======================================================================================================
bool Texture::Load(const std::string& tag, const std::string& filename)
{
	assert(s_textures.find(tag) == s_textures.end());

	//TODO - Make use of a simpler image loading library such as SOIL
	//TODO - Find a way to load the image the right way up, 
	//else we have to add conversion to flip the raw pixel data
	SDL_Surface* textureData = IMG_Load((s_rootFolder + filename).c_str());

	if (!textureData)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error loading texture file \"" + (s_rootFolder + filename) + "\"\n\n"
			"Possible causes could be a corrupt or missing file. Another reason could be "
			"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
		return false;
	}
	//This is all the raw image data 
	auto width = textureData->w;
	auto height = textureData->h;
	auto pixels = reinterpret_cast<Uint8*>(textureData->pixels);
	auto depth = textureData->format->BytesPerPixel;
	auto format = ((depth == 4) ? GL_RGBA : GL_RGB);

	Texture texture;
	texture.m_tag = tag;

	glGenTextures(1, &texture.m_ID);
	glBindTexture(GL_TEXTURE_2D, texture.m_ID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(textureData);
	s_textures[tag] = texture;
	return true;
}
//======================================================================================================
void Texture::Unload(const std::string& tag)
{
	if (!tag.empty())
	{
		auto it = s_textures.find(tag);
		assert(it != s_textures.end());
		glDeleteTextures(1, &it->second.m_ID);
		s_textures.erase(it);
	}
	else
	{
		for (auto& texture : s_textures)
		{
			glDeleteTextures(1, &texture.second.m_ID);
		}

		s_textures.clear();
	}
}
//======================================================================================================
void Texture::SetRootFolder(const std::string& rootFolder)
{
	s_rootFolder = rootFolder;
}
//======================================================================================================
Texture::Texture(const std::string& tag, const std::string& filename)
		:m_tag(tag)
{
	if (!filename.empty())
	{
		Load(tag, filename);
		SetTexture(tag);
	}
	else if (!tag.empty())
	{
		SetTexture(tag);
	}
}
//======================================================================================================
const std::string& Texture::GetTag() const
{
	return m_tag;
}
//======================================================================================================
void Texture::SetWrapping(WrapSetting wrapSetting)
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapSetting));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapSetting));
	glBindTexture(GL_TEXTURE_2D, 0);
}
//======================================================================================================
void Texture::SetTexture(const std::string& tag)
{
	auto it = s_textures.find(tag);
	assert(it != s_textures.end());
	*this = it->second;
}
//======================================================================================================
void Texture::SetFilter(FilterType filterType, FilterSetting filterSetting)
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexParameteri(GL_TEXTURE_2D, 
		static_cast<GLenum>(filterType), static_cast<GLint>(filterSetting));
	glBindTexture(GL_TEXTURE_2D, 0);
}
//======================================================================================================
void Texture::Bind() const
{
	assert(!m_tag.empty());
	assert(m_ID > 0);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}
//======================================================================================================
void Texture::Bind(TextureUnit textureUnit) const
{
	assert(!m_tag.empty());
	assert(m_ID > 0);
	glActiveTexture(static_cast<GLenum>(textureUnit));
	glBindTexture(GL_TEXTURE_2D, m_ID);
}
//======================================================================================================
void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}