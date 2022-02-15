#pragma once

/*===================================================================#
| 'Light' source files last updated on 23 November 2021              |
#====================================================================#
| Class has not been fully tested. No known issues found.            |
#===================================================================*/

#include "GLAD/gl.h"
#include <glm.hpp>
#include "Buffer.h"
#include "Object.h"

//TODO - Refine class such that it follows newer design
//TODO - Add support for spot, directional and point lights

class Light : public Object
{
public:
	Light(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
	virtual ~Light();

	void SetAttenuationLinear(GLfloat linear);
	void SetAttenuationConstant(GLfloat constant);
	void SetAttenuationQuadratic(GLfloat quadratic);
	void SetAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic);

	void SetAmbient(const glm::vec3& ambient);
	void SetAmbient(GLfloat r, GLfloat g, GLfloat b);

	void SetDiffuse(const glm::vec3& diffuse);
	void SetDiffuse(GLfloat r, GLfloat g, GLfloat b);

	void SetSpecular(const glm::vec3& specular);
	void SetSpecular(GLfloat r, GLfloat g, GLfloat b);

	virtual void Render(Shader& shader); /*debug only*/
	virtual void Update(GLfloat deltaTime) { /*debug only*/ }
	virtual void SendToShader(Shader& shader);

private:
	static GLuint s_totalLights;

	GLuint m_lightNumber;

	GLfloat m_attenuationLinear;
	GLfloat m_attenuationConstant;
	GLfloat m_attenuationQuadratic;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	Buffer m_buffer;
};