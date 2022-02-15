#pragma once

/*===================================================================#
| 'Model' source files last updated on 25 October 2021               |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

#include <map>
#include <vector>
#include "Buffer.h"
#include "Material.h"
#include "Object.h"
#include "Texture.h"

#define NOMINMAX

struct Mesh
{
	std::string name;
	std::string materialName;

	std::vector<GLuint> indices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoords;
};

struct VertexGroup
{
	int v = -1;
	int t = -1;
	int n = -1;

	//This is required to be able to store this data type in the unordered map
	//If values are the same then the map will replace or disregard them
	bool operator==(const VertexGroup& other) const
	{
		return (v == other.v && t == other.t && n == other.n);
	}

};

//This is required to allow the unordered map to store the data using a hash table
//I believe the returned value forms the 'index' value used by the hash table
class HashFunction
{

public:
	size_t operator()(const VertexGroup& v) const
	{
		return v.v + v.t + v.n;
	}
};

typedef std::vector<VertexGroup> Face;

class Model : public Object
{

public:
	static bool Load(const std::string& tag,
		const std::string& filename,
		bool isNormalized = false,
		const std::string& defaultMaterial = "Chrome");

	static void Unload(const std::string& tag = "");
	static void SetRootFolder(const std::string& rootFolder);

	Model(const std::string& tag = "",
		const std::string& filename = "",
		bool isNormalized = false,
		const std::string& defaultMaterial = "Chrome");

	virtual ~Model() {}

	const glm::vec3& GetDimension() const;

	void SetModel(const std::string& tag);
	void SetColor(const glm::vec4& color);
	void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	virtual void Render(Shader& shader);
	virtual void Update(GLfloat deltaTime) {}
	virtual void SendToShader(Shader& shader) {}

private:

	static void Normalize(Model& model);
	static void FillBuffers(Model& model);
	static void SortVertexData(Mesh& newMesh, const Mesh& oldMesh,
		const std::vector<Face>& faces);

	glm::vec3 m_dimension;

	//There should always be the same amount 
	//of buffers as there are mesh groups
	std::vector<Mesh> m_meshes;
	std::vector<Buffer> m_buffers;

	//These are all the materials loaded from the .mtl file
	//This is an entire group of materials for the model
	Material m_material;

	static std::string s_rootFolder;
	static std::map<std::string, Model> s_models;

};