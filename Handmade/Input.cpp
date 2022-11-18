#include "Input.h"
#include "Screen.h"

//======================================================================================================
Input* Input::Instance()
{
	static Input* inputObject = new Input();
	return inputObject;
}
//======================================================================================================
bool Input::IsXClicked() const
{
	return isXClicked;
}
//======================================================================================================
bool Input::IsKeyPressed() const
{
	return isKeyPressed;
}
//======================================================================================================
bool Input::IsWindowResized() const
{
	return isWindowResized;
}
//======================================================================================================
bool Input::IsLeftButtonClicked() const
{
	return isLeftButtonClicked;
}
//======================================================================================================
bool Input::IsRightButtonClicked() const
{
	return isRightButtonClicked;
}
//======================================================================================================
bool Input::IsMiddleButtonClicked() const
{
	return isMiddleButtonClicked;
}
//======================================================================================================
const glm::ivec2& Input::GetMouseWheel() const
{
	return mouseWheel;
}
//======================================================================================================
const glm::ivec2& Input::GetMouseMotion() const
{
	return mouseMotion;
}
//======================================================================================================
const glm::ivec2& Input::GetMousePosition() const
{
	return mousePosition;
}
//======================================================================================================
char Input::GetKeyUp() const
{
	return keyUp;
}
//======================================================================================================
char Input::GetKeyDown() const
{
	return keyDown;
}
//======================================================================================================
KeyState Input::GetKeyStates() const
{
	return keyStates;
}
//======================================================================================================
void Input::SetCursorType(CursorType cursorType)
{
	SDL_FreeCursor(cursor);
	cursor = SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(cursorType));
	SDL_SetCursor(cursor);
}
//======================================================================================================-
void Input::SetCursorState(CursorState cursorEnabled, CursorState cursorVisible)
{
	//If mouse cursors are enabled that means they stay within the bounds 
	//of the window. On top of that they can still be visible or hidden
	if (static_cast<bool>(cursorEnabled))
	{
		if (static_cast<bool>(cursorVisible))
		{
			SDL_ShowCursor(static_cast<int>(CursorState::Show));
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		else
		{
			SDL_ShowCursor(static_cast<int>(CursorState::Hide));
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}

	else
	{
		SDL_ShowCursor(static_cast<int>(CursorState::Hide));
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}
//======================================================================================================
void Input::Update()
{
	//This variable is declared locally 
	//so that it has no left over data
	SDL_Event events;

	isXClicked = false;
	isWindowResized = false;
	mouseWheel = glm::ivec2(0);
	mouseMotion = glm::ivec2(0);

	keyStates = SDL_GetKeyboardState(nullptr);

	//We check for events on SDL's event queue and keep 
	//this loop running until all events have been processed
	while (SDL_PollEvent(&events))
	{
		switch (events.type)
		{

		case SDL_WINDOWEVENT:
		{
			if (events.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				isWindowResized = true;
			}

			break;
		}

		case SDL_QUIT:
		{
			isXClicked = true;
			break;
		}

		case SDL_KEYUP:
		{
			isKeyPressed = false;
			keyUp = events.key.keysym.sym;
			break;
		}

		case SDL_KEYDOWN:
		{
			isKeyPressed = true;
			keyUp = events.key.keysym.sym;
			break;
		}

		case SDL_MOUSEMOTION:
		{
			mousePosition.x = events.motion.x;
			mousePosition.y = events.motion.y;
			mouseMotion.x = events.motion.xrel;
			mouseMotion.y = events.motion.yrel;
			break;
		}

		case SDL_MOUSEWHEEL:
		{
			mouseWheel.x = events.wheel.x;
			mouseWheel.y = events.wheel.y;
			break;
		}

		case SDL_MOUSEBUTTONUP:
		{
			switch (events.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				isLeftButtonClicked = false;
				break;
			}

			case SDL_BUTTON_RIGHT:
			{
				isRightButtonClicked = false;
				break;
			}

			case SDL_BUTTON_MIDDLE:
			{
				isMiddleButtonClicked = false;
				break;
			}
			}

			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			switch (events.button.button)
			{
			case SDL_BUTTON_LEFT: 
			{ 
				isLeftButtonClicked = true; 
				break; 
			}

			case SDL_BUTTON_RIGHT: 
			{ 
				isRightButtonClicked = true; 
				break; 
			}

			case SDL_BUTTON_MIDDLE: 
			{ 
				isMiddleButtonClicked = true; 
				break; 
			}
			}

			break;
		}
		}
	}
}