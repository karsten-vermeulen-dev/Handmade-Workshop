#pragma once

/*===================================================================#
| 'Material' source files last updated on 23 November 2021           |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

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
	static bool Load(const std::string& tag, const std::string& filename);
	static void Unload(const std::string& tag = "");
	static void SetRootFolder(const std::string& rootFolder);

	Material(const std::string& tag = "", const std::string& filename = "");
	~Material() {}

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

	void SendToShader(Shader& shader);

private:
	bool m_isTextured = false;
	std::string m_tag;
	std::string m_name;
	std::vector<Material> m_group;

	GLfloat m_shininess = 1.0f;
	GLfloat m_refractiveIndex = 0.0f;

	Texture m_normalMap;
	Texture m_ambientMap;
	Texture m_diffuseMap;
	Texture m_specularMap;

	glm::vec3 m_ambient = glm::vec3(0.0f);
	glm::vec3 m_diffuse	= glm::vec3(0.0f);
	glm::vec3 m_emission = glm::vec3(0.0f);
	glm::vec3 m_specular = glm::vec3(0.0f);
	glm::vec3 m_transmittance = glm::vec3(1.0f);

	static std::string s_rootFolder;
	static std::map<std::string, std::vector<Material>> s_materialGroups;
};