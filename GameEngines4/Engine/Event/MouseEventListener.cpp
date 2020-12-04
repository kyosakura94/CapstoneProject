#include "MouseEventListener.h"
#include "../Core/CoreEngine.h"

CoreEngine* MouseEventListener::engineInstance = nullptr;
vec2 MouseEventListener::curMouse = vec2();
vec2 MouseEventListener::preMouse = vec2();
bool MouseEventListener::firstUpdate = true;

void MouseEventListener::RegisterEngineObject(CoreEngine * engine_)
{
	engineInstance = engine_;
}

void MouseEventListener::Update(SDL_Event e_)
{
	if (e_.type == SDL_MOUSEBUTTONDOWN)
	{
		UpdateMousePosition();
		NotifyOfMousePressed(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEBUTTONUP)
	{
		UpdateMousePosition();
		NotifyOfMouseReleased(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEMOTION )
	{
		UpdateMousePosition();
		NotifyOfMouseMove();
	} 
	else if (e_.type == SDL_MOUSEWHEEL)
	{
		NotifyOfMouseScroll(e_.wheel.y);
	}
}

void MouseEventListener::Update(SDL_Event e_, const Uint8* keystate)
{
	if (e_.type == SDL_MOUSEBUTTONDOWN)
	{
		UpdateMousePosition();
		NotifyOfMousePressed(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEBUTTONUP)
	{
		UpdateMousePosition();
		NotifyOfMouseReleased(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEMOTION && keystate[SDL_SCANCODE_LALT])
	{
		cout << "move" << endl;
		UpdateMousePosition();
		NotifyOfMouseMove();
	}
	else if (e_.type == SDL_MOUSEWHEEL)
	{
		NotifyOfMouseScroll(e_.wheel.y);
	}

}

MouseEventListener::~MouseEventListener()
{
	engineInstance = nullptr;
}

void MouseEventListener::NotifyOfMousePressed(int buttonType_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMousePressed(curMouse);
	}
}

void MouseEventListener::NotifyOfMouseReleased(int butonType_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseReleased(curMouse, butonType_);
	}
}

void MouseEventListener::NotifyOfMouseMove()
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseMove(curMouse);
	}
}

void MouseEventListener::NotifyOfMouseScroll(int y_)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfMouseScroll(y_);
	}
}

void MouseEventListener::NotifyOfKeyPress(SDL_KeyboardEvent* key)
{
	if (engineInstance)
	{
		engineInstance->NotifyOfKeyPress();
	}
}

bool MouseEventListener::ClickButton()
{
	return false;
}

vec2 MouseEventListener::GetMousePosition()
{
	return curMouse;
}

vec2 MouseEventListener::GetPreviousMousePosition()
{
	return preMouse;
}

vec2 MouseEventListener::GetMouseOffset()
{
	return vec2(curMouse.x - preMouse.x, preMouse.y - curMouse.y);

}

void MouseEventListener::UpdateMousePosition()
{
	int tempX, tempY;
	SDL_GetMouseState(&tempX, &tempY);
	tempY = CoreEngine::GetInstance()->GetWindowSize().y - tempY;

	if (firstUpdate)
	{
		preMouse.x = curMouse.x = tempX;
		preMouse.y = curMouse.y = tempY;
		firstUpdate = false;
	}
	else
	{
		preMouse = curMouse;
		curMouse.x = tempX;
		curMouse.y = tempY;
	}
}
