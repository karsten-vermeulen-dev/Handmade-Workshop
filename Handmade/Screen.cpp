#include <assert.h>
#include <map>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include "Screen.h"
#include "Utility.h"

//======================================================================================================
Screen* Screen::Instance()
{
	static Screen* screenObject = new Screen();
	return screenObject;
}
//======================================================================================================
Screen::Screen()
{
	m_window = nullptr;
	m_context = nullptr;
	m_resolution = glm::ivec2(0);
}
//======================================================================================================
bool Screen::Initialize(const std::string& filename)
{
	std::map<std::string, std::string> dataMap;

	if (!Utility::LoadConfigFile(filename, dataMap))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Error opening config file '" + filename + "'", Utility::Severity::Failure);
		return false;
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"SDL did not initialize properly.", Utility::Severity::Failure);
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Set up anti-aliasing 
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	GLint profile = std::stoi(dataMap["Core"]) == 1 ? SDL_GL_CONTEXT_PROFILE_CORE
		: SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;

	//set OpenGL context and profile using values assigned earlier
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, std::stoi(dataMap["Major"]));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, std::stoi(dataMap["Minor"]));

	auto screenFlag = std::stoi(dataMap["Maximized"]) == 1 
		? SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
		: SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	m_window = SDL_CreateWindow(dataMap["Name"].c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		std::stoi(dataMap["Width"]),
		std::stoi(dataMap["Height"]),
		screenFlag);

	if (!m_window)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Game window could not be created.", Utility::Severity::Failure);
		return false;
	}

	m_context = SDL_GL_CreateContext(m_window);

	if (!m_context)
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"OpenGL context could not be created. "
			"The context is either invalid or not supported by your graphics card",
			Utility::Severity::Failure);
		return false;
	}

	if (!gladLoaderLoadGL())
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"GLAD could not be initialized", Utility::Severity::Failure);
		return false;
	}

	//TODO - Not sure if we need this and where it should go
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	m_resolution.x = std::stoi(dataMap["Width"]);
	m_resolution.y = std::stoi(dataMap["Height"]);

	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_SCISSOR_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);

	return true;
}
//======================================================================================================
const glm::ivec2& Screen::GetResolution() 
{
	SDL_GetWindowSize(m_window, &m_resolution.x, &m_resolution.y);
	assert(m_resolution != glm::ivec2(0));
	return m_resolution;
}
//======================================================================================================
void Screen::SetVSync(VSync VSync)
{
	if (SDL_GL_SetSwapInterval(static_cast<int>(VSync) == -1))
	{
		Utility::Log(Utility::Destination::WindowsMessageBox,
			"Vertical sync not supported", Utility::Severity::Warning);
	}
}
//======================================================================================================
void Screen::IsDepthTestEnabled(bool flag)
{
	flag ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}
//======================================================================================================
void Screen::SetCursorPosition(GLuint x, GLuint y)
{
	SDL_WarpMouseInWindow(m_window, x, y);
}
//======================================================================================================
void Screen::SetResolution(GLint width, GLint height)
{
	m_resolution.x = width;
	m_resolution.y = height;
	glm::max(m_resolution, 1);
}
//======================================================================================================
void Screen::SetColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}
//======================================================================================================
void Screen::SetColor(const glm::uvec4& color)
{
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
}
//======================================================================================================
void Screen::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
}
//======================================================================================================
void Screen::SetColor(GLuint r, GLuint g, GLuint b, GLuint a)
{
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
//======================================================================================================
void Screen::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);
	glScissor(x, y, width, height);
}
//======================================================================================================
void Screen::Refresh()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//======================================================================================================
void Screen::Present()
{
	SDL_GL_SwapWindow(m_window);
}
//======================================================================================================
void Screen::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}