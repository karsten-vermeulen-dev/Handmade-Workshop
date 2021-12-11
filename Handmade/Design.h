#pragma once

/*===================================================================#
| 'Design' source files last updated on 8 December 2021              |
#===================================================================*/

#include <deque>
#include <memory>
#include <vector>
#include <glm.hpp>
#include "Audio.h"
#include "Axes.h"
#include "Cuboid.h"
#include "FreeCamera.h"
#include "Grid.h"
#include "Light.h"
#include "Model.h"
#include "Object.h"
#include "Quad.h"
#include "Sphere.h"
#include "Shader.h"
#include "State.h"
#include "Text.h"

const auto MINOR = 0.2f;
const auto MAJOR = 0.8f;
const auto UI_PADDING = 2.0f;
const auto MAX_CONSOLE_LOG_SIZE = 1000;

class Design : public State
{

public:

	Design();
	virtual ~Design() {}

	virtual bool OnEnter();
	virtual State* Update(int deltaTime);
	virtual bool Render();
	virtual void OnExit();

private:

	void RenderConsoleWindow();
	void RenderPropertiesWindow();

	GLint m_minorWidth;
	GLint m_majorWidth;
	GLint m_minorHeight;
	GLint m_majorHeight;

	glm::ivec2 m_resolution;
	glm::vec3 m_sceneRotation;

	std::unique_ptr<Grid> m_grid;
	std::unique_ptr<Axes> m_axes;

	std::unique_ptr<Text> m_topText;
	std::unique_ptr<Text> m_bottomText;
	std::unique_ptr<Text> m_axesLabelText;

	std::unique_ptr<Shader> m_mainShader;
	std::unique_ptr<Shader> m_textShader;
	std::unique_ptr<Shader> m_lightShader;
	std::unique_ptr<Shader> m_testShader;
	std::unique_ptr<FreeCamera> m_sceneCamera;

	//For current testing===============================================
	/*std::unique_ptr<Audio> m_audio1;
	std::unique_ptr<Audio> m_audio2;
	std::unique_ptr<Audio> m_audio3;*/

	std::unique_ptr<Light> m_light;
	//std::unique_ptr<Model> m_model;

	std::unique_ptr<Quad> m_quad;
	//std::unique_ptr<Cuboid> m_cube;
	//std::unique_ptr<Sphere> m_sphere;

	//std::unique_ptr<Tile> m_labelX;
	//std::unique_ptr<Tile> m_labelY;
	//std::unique_ptr<Tile> m_labelZ;

	//Using static objects will enforce more copies being made 
	//especially if the vector container has not been reserved
	//Prefer to use smart pointer objects instead to avoid copies
	//std::vector<Text> m_text;
	//std::vector<Audio> m_audio;
	//==================================================================

	std::deque<std::string> m_consoleLog;
	std::vector<std::unique_ptr<Object>> m_objects;
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float A = 0.0f;
	float S = 0.0f; //Scale
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	//std::unique_ptr<Cuboid> m_cube;
};