#pragma once
#include <map>
#include <string>
#include <vector>
#include "GLAD/gl.h"
#include <glm.hpp>
#include "Shader.h"
#include "Texture.h"

class Material
{

public:

	Material(const std::string& tag = "", const std::string& filename = "");

	bool Load(const std::string& tag, const std::string& filename);
	void Unload(const std::string& tag = "");

	const std::string& GetTag() const;
	const std::string& GetName() const;
	const Texture& GetNormalMap() const;
	const Texture& GetAmbientMap() const;
	const Texture& GetDiffuseMap() const;
	const Texture& GetSpecularMap() const;
	const std::vector<Material>& GetGroup() const;

	void SetName(const std::string& name);
	void SetGroup(const std::string& tag);

	bool IsTextured() const;

	void LoadNormalMap(const std::string& tag, const std::string& filename);
	void LoadAmbientMap(const std::string& tag, const std::string& filename);
	void LoadDiffuseMap(const std::string& tag, const std::string& filename);
	void LoadSpecularMap(const std::string& tag, const std::string& filename);

	void SetShininess(GLfloat shininess);
	void SetRefractiveIndex(GLfloat refractiveIndex);

	void SetAmbient(const glm::vec3& ambient);
	void SetAmbient(GLfloat r, GLfloat g, GLfloat b);

	void SetDiffuse(const glm::vec3& diffuse);
	void SetDiffuse(GLfloat r, GLfloat g, GLfloat b);

	void SetSpecular(const glm::vec3& specular);
	void SetSpecular(GLfloat r, GLfloat g, GLfloat b);

	void SetEmission(const glm::vec3& emission);
	void SetEmission(GLfloat r, GLfloat g, GLfloat b);

	void SetTransmittance(const glm::vec3& transmittance);
	void SetTransmittance(GLfloat r, GLfloat g, GLfloat b);

	void SendToShader(Shader& shader) const;

private:

	std::string tag;
	std::string name;
	bool isTextured{ false };
	std::vector<Material> group;

	Texture normalMap;
	Texture ambientMap;
	Texture diffuseMap;
	Texture specularMap;

	GLfloat shininess{ 1.0f };
	GLfloat refractiveIndex{ 0.0f };

	glm::vec3 ambient{ 0.0f };
	glm::vec3 diffuse{ 0.0f };
	glm::vec3 specular{ 0.0f };
	glm::vec3 emission{ 0.0f };
	glm::vec3 transmittance{ 1.0f };

	static std::string rootFolder;
	static std::map<std::string, std::vector<Material>> materialGroups;

};