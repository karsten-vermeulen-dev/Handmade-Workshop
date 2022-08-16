#pragma once
#include <deque>
#include <memory>
#include <vector>
#include "GLAD/gl.h"
#include <glm.hpp>
#include "Audio.h"
#include "Axes.h"
#include "Circle.h"
#include "Cuboid.h"
#include "FreeCamera.h"
#include "Grid.h"
#include "Light.h"
#include "Model.h"
#include "Object.h"
#include "Quad.h"
#include "Shader.h"
#include "Sphere.h"
#include "State.h"
#include "Text.h"

class Design : public State
{

public:

	virtual ~Design() {}

	virtual bool OnEnter();
	virtual State* Update(int deltaTime);
	virtual bool Render();
	virtual void OnExit();

private:

	const GLfloat minor{ 0.15f };
	const GLfloat major{ 0.85f };
	const GLfloat fontSize{ 18.0f };
	const GLfloat paddingUI{ 2.0f };
	const GLuint menuBarHeight{ 24 };
	const GLuint maxConsoleLogSize{ 1000 };

	void RenderMenu();
	void RenderConsoleWindow();
	void RenderHierarchyWindow();
	void RenderPropertiesWindow();

	GLint minorWidth{ 0 };
	GLint majorWidth{ 0 };
	GLint minorHeight{ 0 };
	GLint majorHeight{ 0 };

	bool isStateComplete{ false };

	glm::ivec2 resolution{ 0U };
	glm::vec3 sceneRotation{ 45.0f, -30.0f, 0.0f };

	std::unique_ptr<Grid> grid;
	std::unique_ptr<Axes> axes;

	std::unique_ptr<Text> topText;
	std::unique_ptr<Text> bottomText;
	std::unique_ptr<Text> axesLabelText;

	std::unique_ptr<Shader> mainShader;
	std::unique_ptr<Shader> textShader;
	std::unique_ptr<Shader> lightShader;
	std::unique_ptr<Shader> testShader;

	std::unique_ptr<FreeCamera> sceneCamera;

	//For current testing===============================================
	/*std::unique_ptr<Audio> audio1;
	std::unique_ptr<Audio> audio2;
	std::unique_ptr<Audio> audio3;*/

	std::unique_ptr<Light> light;

	//Generic object (for testing)
	Object* activeObject{ nullptr };
	std::vector<std::unique_ptr<Object>> objects;

	//std::unique_ptr<Tile> labelX;
	//std::unique_ptr<Tile> labelY;
	//std::unique_ptr<Tile> labelZ;

	//Using static objects will enforce more copies being made 
	//especially if the vector container has not been reserved
	//Prefer to use smart pointer objects instead to avoid copies
	//std::vector<Text> text;
	//std::vector<Audio> audio;
	//==================================================================

	std::deque<std::string> consoleLog;

};