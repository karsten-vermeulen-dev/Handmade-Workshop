#include <assert.h>
#include <iostream>
#include <SDL_image.h>
#include "Texture.h"
#include "Utility.h"

std::map<std::string, Texture> Texture::textures;
std::string Texture::rootFolder = "Assets/Textures/";

//======================================================================================================
Texture::Texture(const std::string& tag, const std::string& filename)
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
bool Texture::Load(const std::string& tag, const std::string& filename)
{
	assert(textures.find(tag) == textures.end());

	//TODO - Make use of a simpler image loading library such as SOIL
	//TODO - Find a way to load the image the right way up, 
	//else we have to add conversion to flip the raw pixel data
	SDL_Surface* textureData = IMG_Load((rootFolder + filename).c_str());

	if (!textureData)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error loading texture file \"" + (rootFolder + filename) + "\"\n\n"
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

	this->tag = tag;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(textureData);
	textures[tag] = *this;
	return true;
}
//======================================================================================================
void Texture::Unload(const std::string& tag)
{
	if (!tag.empty())
	{
		auto it = textures.find(tag);
		assert(it != textures.end());
		glDeleteTextures(1, &it->second.ID);
		textures.erase(it);
	}

	else
	{
		for (auto& texture : textures)
		{
			glDeleteTextures(1, &texture.second.ID);
		}

		textures.clear();
	}
}
//======================================================================================================
const std::string& Texture::GetTag() const
{
	return tag;
}
//======================================================================================================
void Texture::SetTexture(const std::string& tag)
{
	auto it = textures.find(tag);
	assert(it != textures.end());
	*this = it->second;
}
//======================================================================================================
void Texture::SetWrapping(WrapSetting wrapSetting) const
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapSetting));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapSetting));
	glBindTexture(GL_TEXTURE_2D, 0);
}
//======================================================================================================
void Texture::SetFilter(FilterType filterType, FilterSetting filterSetting) const
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D,
		static_cast<GLenum>(filterType), static_cast<GLint>(filterSetting));
	glBindTexture(GL_TEXTURE_2D, 0);
}
//======================================================================================================
void Texture::Bind() const
{
	assert(!tag.empty());
	assert(ID > 0);
	glBindTexture(GL_TEXTURE_2D, ID);
}
//======================================================================================================
void Texture::Bind(TextureUnit textureUnit) const
{
	assert(!tag.empty());
	assert(ID > 0);
	glActiveTexture(static_cast<GLenum>(textureUnit));
	glBindTexture(GL_TEXTURE_2D, ID);
}
//======================================================================================================
void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}