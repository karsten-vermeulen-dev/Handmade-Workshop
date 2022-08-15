#include <assert.h>
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Screen.h"
#include "Shader.h"
#include "Text.h"

//======================================================================================================
App::App(State* initialState)
{
	appState.reset(initialState);

	if (Initialize())
	{
		if (CreateObjects())
		{
			//??
		}
	}
}
//======================================================================================================
App::~App()
{
	Shutdown();
}
//======================================================================================================
void App::Run()
{
	if (appState)
	{
		appState->OnEnter();
	}

	while (appState)
	{
		int startTime = SDL_GetTicks();

		Screen::Instance()->Refresh();
		Input::Instance()->Update();

		State* nextState = appState->Update(deltaTime);

		appState->Render();

		Screen::Instance()->Present();

		if (nextState != appState.get())
		{
			appState->OnExit();
			appState.reset(nextState);

			if (appState)
			{
				appState->OnEnter();
			}
		}

		deltaTime = SDL_GetTicks() - startTime;
	}
}
//======================================================================================================
bool App::Initialize()
{
	if (!Screen::Instance()->Initialize("Data/Config.ini"))
	{
		return false;
	}

	if (!Shader::Initialize())
	{
		return false;
	}

	if (!Text::Initialize())
	{
		return false;
	}

	if (!Audio::Initialize())
	{
		return false;
	}

	return true;
}
//======================================================================================================
bool App::CreateObjects()
{
	return true;
}
//======================================================================================================
void App::Shutdown()
{
	Shader::Shutdown();
	Screen::Instance()->Shutdown();
}