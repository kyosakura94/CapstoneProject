#include "KeyEventListener.h"
CoreEngine* KeyEventListener::engineInstance = nullptr;
const char* KeyEventListener::keyname = "";

void KeyEventListener::RegisterEngineObject(CoreEngine* engine_)
{
	engineInstance = engine_;
}

bool KeyEventListener::GetKeyState(std::string keyname_)
{
	if (keyname_ == keyname)
	{
		keyname = "";
		return true;
	}

	return false;
}

void KeyEventListener::SetKeyState(SDL_KeyboardEvent* key)
{
	keyname = SDL_GetKeyName(key->keysym.sym);

	//if (key->type == SDL_KEYUP)
	//	keyname = "";
	//else
		
	
}

KeyEventListener::~KeyEventListener()
{
	engineInstance = nullptr;
}
