#pragma once
#include <map>
#include <string>
#include "GLAD/gl.h"

class Texture
{

public:

	enum class WrapSetting
	{
		Repeat = GL_REPEAT,
		RepeatMirror = GL_MIRRORED_REPEAT,
		Clamp = GL_CLAMP_TO_EDGE
	};

	enum class FilterType
	{
		Minification = GL_TEXTURE_MIN_FILTER,
		Magnification = GL_TEXTURE_MAG_FILTER,
		Anisotropic = GL_TEXTURE_MAX_ANISOTROPY
	};

	enum class FilterSetting
	{
		Nearest = GL_NEAREST,
		Bilinear = GL_LINEAR,
		Trilinear = GL_LINEAR_MIPMAP_LINEAR,
		Anisotropic4 = 4,
		Anisotropic8 = 8,
		Anisotropic16 = 16
	};

	enum class TextureUnit
	{
		Texture1 = GL_TEXTURE0,
		Texture2 = GL_TEXTURE1,
		Texture3 = GL_TEXTURE2,
		Texture4 = GL_TEXTURE3
	};

	Texture(const std::string& tag = "", const std::string& filename = "");

	bool Load(const std::string& tag, const std::string& filename);
	void Unload(const std::string& tag = "");

	const std::string& GetTag() const;

	void SetTexture(const std::string& tag);
	void SetWrapping(WrapSetting wrapSetting) const;
	void SetFilter(FilterType filterType, FilterSetting filterSetting) const;

	void Bind() const;
	void Bind(TextureUnit textureUnit) const;
	void Unbind() const;

private:

	GLuint ID{ 0 };
	std::string tag;

	static std::string rootFolder;
	static std::map<std::string, Texture> textures;

};