#include <assert.h>
#include <fstream>
#include "Material.h"
#include "Shader.h"
#include "Utility.h"

std::string Material::rootFolder = "Assets/Materials/";
std::map<std::string, std::vector<Material>> Material::materialGroups;

//======================================================================================================
bool Material::Load(const std::string& tag, const std::string& filename)
{
	assert(materialGroups.find(tag) == materialGroups.end());
	std::fstream file(rootFolder + filename, std::ios_base::in);

	if (!file)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error loading material file \"" + (rootFolder + filename) + "\"\n\n"
			"Possible causes could be a corrupt or missing file. Another reason could be "
			"that the filename and/or path are incorrectly spelt.", Utility::Severity::Failure);
		return false;
	}

	this->tag = tag;
	group.reserve(5);

	std::string line;
	std::vector<std::string> subStrings;
	subStrings.reserve(5);

	while (!file.eof())
	{
		getline(file, line);
		subStrings.clear();

		if (!line.empty())
		{
			Utility::ParseString(line, subStrings, ' ');

			//Add the material into the container. All properties from now will add to this
			//material which is the start of a material grouping matching the OBJ grouping
			if (subStrings[0] == "newmtl")
			{
				group.push_back(Material());
				group.back().SetName(subStrings[1]);
				continue;
			}

			//Ambient RGB values
			if (subStrings[0] == "Ka")
			{
				group.back().SetAmbient(std::stof(subStrings[1]),
					std::stof(subStrings[2]),
					std::stof(subStrings[3]));
				continue;
			}

			//Diffuse RGB values
			if (subStrings[0] == "Kd")
			{
				group.back().SetDiffuse(std::stof(subStrings[1]),
					std::stof(subStrings[2]),
					std::stof(subStrings[3]));
				continue;
			}

			//Specular RGB values
			if (subStrings[0] == "Ks")
			{
				group.back().SetSpecular(std::stof(subStrings[1]),
					std::stof(subStrings[2]),
					std::stof(subStrings[3]));
				continue;
			}

			//Transmittance RGB values which is the ratio of the light 
			//energy falling on a surface to that transmitted through it
			if (subStrings[0] == "Kt")
			{
				group.back().SetTransmittance(std::stof(subStrings[1]),
					std::stof(subStrings[2]),
					std::stof(subStrings[3]));
				continue;
			}

			//Emission RGB values
			if (subStrings[0] == "Ke")
			{
				group.back().SetEmission(std::stof(subStrings[1]),
					std::stof(subStrings[2]),
					std::stof(subStrings[3]));
				continue;
			}

			//Refractive index scalar value which is the measure of the 
			//bending of light when passing from one medium into another
			if (subStrings[0] == "Ni")
			{
				group.back().SetRefractiveIndex(std::stof(subStrings[1]));
				continue;
			}

			//Specular exponent or shininess value (possible 0 - 1000)
			if (subStrings[0] == "Ns")
			{
				group.back().SetShininess(std::stof(subStrings[1]));
				continue;
			}

			//Ambient texture file
			if (subStrings[0] == "map_Ka")
			{
				group.back().LoadAmbientMap(subStrings[1], subStrings[1]);
				continue;
			}

			//Diffuse texture file
			if (subStrings[0] == "map_Kd")
			{
				group.back().LoadDiffuseMap(subStrings[1], subStrings[1]);
				continue;
			}

			//Specular texture file
			if (subStrings[0] == "map_Ks")
			{
				group.back().LoadSpecularMap(subStrings[1], subStrings[1]);
				continue;
			}

			//Normal texture file
			if (subStrings[0] == "map_Ns" || subStrings[0] == "bump")
			{
				group.back().LoadNormalMap(subStrings[1], subStrings[1]);
				continue;
			}
		}
	}

	file.close();
	materialGroups[tag] = group;
	return true;
}
//======================================================================================================
void Material::Unload(const std::string& tag)
{
	if (!tag.empty())
	{
		auto it = materialGroups.find(tag);
		assert(it != materialGroups.end());

		for (const auto& material : it->second)
		{
			//TODO - Find a different way to unload the textures
			//Texture::Unload(material.GetAmbientMap().GetTag());
			//Texture::Unload(material.GetDiffuseMap().GetTag());
			//Texture::Unload(material.GetSpecularMap().GetTag());
			//Texture::Unload(material.GetNormalMap().GetTag());
		}

		materialGroups.erase(it);
	}

	else
	{
		for (auto& material : materialGroups)
		{
			for (const auto& mat : material.second)
			{
				//TODO - Find a different way to unload the textures
				//Texture::Unload(mat.GetAmbientMap().GetTag());
				//Texture::Unload(mat.GetDiffuseMap().GetTag());
				//Texture::Unload(mat.GetSpecularMap().GetTag());
				//Texture::Unload(mat.GetNormalMap().GetTag());
			}
		}

		materialGroups.clear();
	}
}
//======================================================================================================
Material::Material(const std::string& tag, const std::string& filename)
{
	if (!filename.empty())
	{
		Load(tag, filename);
		SetGroup(tag);
	}

	else if (!tag.empty())
	{
		SetGroup(tag);
	}
}
//======================================================================================================
const std::string& Material::GetTag() const
{
	return tag;
}
//======================================================================================================
const std::string& Material::GetName() const
{
	return name;
}
//======================================================================================================
const Texture& Material::GetNormalMap() const
{
	return normalMap;
}
//======================================================================================================
const Texture& Material::GetAmbientMap() const
{
	return ambientMap;
}
//======================================================================================================
const Texture& Material::GetDiffuseMap() const
{
	return diffuseMap;
}
//======================================================================================================
const Texture& Material::GetSpecularMap() const
{
	return specularMap;
}
//======================================================================================================
const std::vector<Material>& Material::GetGroup() const
{
	return group;
}
//======================================================================================================
void Material::SetName(const std::string& name)
{
	this->name = name;
}
//======================================================================================================
void Material::SetGroup(const std::string& tag)
{
	auto it = materialGroups.find(tag);
	assert(it != materialGroups.end());
	group = it->second;
}
//======================================================================================================
bool Material::IsTextured() const
{
	return isTextured;
}
//======================================================================================================
void Material::LoadNormalMap(const std::string& tag, const std::string& filename)
{
	normalMap.Load(tag, filename);
	normalMap.SetTexture(tag);
	isTextured = true;
}
//======================================================================================================
void Material::LoadAmbientMap(const std::string& tag, const std::string& filename)
{
	ambientMap.Load(tag, filename);
	ambientMap.SetTexture(tag);
	isTextured = true;
}
//======================================================================================================
void Material::LoadDiffuseMap(const std::string& tag, const std::string& filename)
{
	diffuseMap.Load(tag, filename);
	diffuseMap.SetTexture(tag);
	isTextured = true;
}
//======================================================================================================
void Material::LoadSpecularMap(const std::string& tag, const std::string& filename)
{
	specularMap.Load(tag, filename);
	specularMap.SetTexture(tag);
	isTextured = true;
}
//======================================================================================================
void Material::SetShininess(GLfloat shininess)
{
	this->shininess = shininess;
}
//======================================================================================================
void Material::SetRefractiveIndex(GLfloat refractiveIndex)
{
	this->refractiveIndex = refractiveIndex;
}
//======================================================================================================
void Material::SetAmbient(const glm::vec3& ambient)
{
	this->ambient = ambient;
}
//======================================================================================================
void Material::SetAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	ambient.r = r;
	ambient.g = g;
	ambient.b = b;
}
//======================================================================================================
void Material::SetDiffuse(const glm::vec3& diffuse)
{
	this->diffuse = diffuse;
}
//======================================================================================================
void Material::SetDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	diffuse.r = r;
	diffuse.g = g;
	diffuse.b = b;
}
//======================================================================================================
void Material::SetSpecular(const glm::vec3& specular)
{
	this->specular = specular;
}
//======================================================================================================
void Material::SetSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	specular.r = r;
	specular.g = g;
	specular.b = b;
}
//======================================================================================================
void Material::SetEmission(const glm::vec3& emission)
{
	this->emission = emission;
}
//======================================================================================================
void Material::SetEmission(GLfloat r, GLfloat g, GLfloat b)
{
	emission.r = r;
	emission.g = g;
	emission.b = b;
}
//======================================================================================================
void Material::SetTransmittance(const glm::vec3& transmittance)
{
	this->transmittance = transmittance;
}
//======================================================================================================
void Material::SetTransmittance(GLfloat r, GLfloat g, GLfloat b)
{
	transmittance.r = r;
	transmittance.g = g;
	transmittance.b = b;
}
//======================================================================================================
void Material::SendToShader(Shader& shader) const
{
	//TODO - Need to tag each material
	//assert(!tag.empty());
	shader.SendData("material.ambient", ambient);
	shader.SendData("material.diffuse", diffuse);
	shader.SendData("material.specular", specular);
	shader.SendData("material.shininess", shininess);
}