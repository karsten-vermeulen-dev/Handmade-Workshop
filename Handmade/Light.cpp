#include "Light.h"

GLuint Light::totalLights = 0;

//======================================================================================================
int Light::GetTotalLights()
{
	return totalLights;
}
//======================================================================================================
Light::Light(const std::string& tag, GLfloat x, GLfloat y, GLfloat z) : Object(tag), buffer(tag, 1)
{
	totalLights++;
	transform.SetPosition(x, y, z);

	//TODO - Find a way to only create one single 
	//buffer to be shared amongst subsequent lights

	GLfloat vertex[] = { 0.0f, 0.0f, 0.0f };
	GLfloat color[] = { ambient.r,  ambient.g,  ambient.b, 1.0f };

	buffer.FillVBO(Buffer::VBO::VertexBuffer, vertex, sizeof(vertex), Buffer::Fill::Ongoing);
	buffer.FillVBO(Buffer::VBO::ColorBuffer, color, sizeof(color), Buffer::Fill::Ongoing);
}
//======================================================================================================
Light::~Light()
{
	buffer.Destroy(tag);
	totalLights--;
}
//======================================================================================================
void Light::SetAttenuationLinear(GLfloat linear)
{
	attenuationLinear = linear;
}
//======================================================================================================
void Light::SetAttenuationConstant(GLfloat constant)
{
	attenuationConstant = constant;
}
//======================================================================================================
void Light::SetAttenuationQuadratic(GLfloat quadratic)
{
	attenuationQuadratic = quadratic;
}
//======================================================================================================
void Light::SetAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic)
{
	attenuationLinear = linear;
	attenuationConstant = constant;
	attenuationQuadratic = quadratic;
}
//======================================================================================================
void Light::SetAmbient(const glm::vec3& ambient)
{
	this->ambient = ambient;
}
//======================================================================================================
void Light::SetAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	//The ambient color will also be the color of the actual light bulb (for now)
	ambient.r = r;
	ambient.g = g;
	ambient.b = b;
	buffer.FillVBO(Buffer::VBO::ColorBuffer, &ambient.r, sizeof(ambient), Buffer::Fill::Ongoing);
}
//======================================================================================================
void Light::SetDiffuse(const glm::vec3& diffuse)
{
	this->diffuse = diffuse;
}
//======================================================================================================
void Light::SetDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	diffuse.r = r;
	diffuse.g = g;
	diffuse.b = b;
}
//======================================================================================================
void Light::SetSpecular(const glm::vec3& specular)
{
	this->specular = specular;
}
//======================================================================================================
void Light::SetSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	specular.r = r;
	specular.g = g;
	specular.b = b;
}
//======================================================================================================
void Light::Render(Shader& shader)
{
	//TODO - Find a way to do this only once
	buffer.LinkVBO(shader.GetAttributeID("vertexIn"),
		Buffer::VBO::VertexBuffer, Buffer::ComponentSize::XYZ, Buffer::DataType::FloatData);
	buffer.LinkVBO(shader.GetAttributeID("colorIn"),
		Buffer::VBO::ColorBuffer, Buffer::ComponentSize::RGBA, Buffer::DataType::FloatData);

	Buffer::SetPointSize(25.0f);
	buffer.Render(Buffer::RenderMode::Points);
}
//======================================================================================================
void Light::SendToShader(Shader& shader)
{
	//Shader::Instance()->SendData("totalLights",  totalLights);
	shader.SendData("model", transform.GetMatrix());
	shader.SendData("light.ambient", ambient);
	shader.SendData("light.diffuse", diffuse);
	shader.SendData("light.specular", specular);
	shader.SendData("light.position", transform.GetPosition());

	//Shader::Instance()->SendData("light.attenuationLinear",  attenuationLinear);
	//Shader::Instance()->SendData("light.attenuationConstant",  attenuationConstant);
	//Shader::Instance()->SendData("light.attenuationQuadratic",  attenuationQuadratic);

	//For passing in an array of lights
	/*Shader::Instance()->SendData("light[" +
		std::to_string( lightNumber) + "].ambient",  ambient);
	Shader::Instance()->SendData("light[" +
		std::to_string( lightNumber) + "].diffuse",  diffuse);
	Shader::Instance()->SendData("light[" +
		std::to_string( lightNumber) + "].specular",  specular);
	Shader::Instance()->SendData("light[" +
		std::to_string( lightNumber) + "].position",  transform.GetPosition());

	Shader::Instance()->SendData("light[" +
		std::to_string( lightNumber) + "].attLinear",  attenuationLinear);
	Shader::Instance()->SendData("light[" +
		std::to_string( lightNumber) + "].attConstant",  attenuationConstant);
	Shader::Instance()->SendData("light[" +
		std::to_string( lightNumber) + "].attQuadratic",  attenuationQuadratic);*/
}