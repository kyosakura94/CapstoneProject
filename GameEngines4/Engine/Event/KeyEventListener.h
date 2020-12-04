#ifndef KEYEVENTLISTENER_H
#define KEYEVENTLISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>
#include <string>

class CoreEngine;
using namespace glm;
class KeyEventListener
{
public:
	KeyEventListener(const KeyEventListener&) = delete;
	KeyEventListener(KeyEventListener&&) = delete;
	KeyEventListener& operator = (const KeyEventListener&) = delete;
	KeyEventListener& operator = (KeyEventListener&&) = delete;

	KeyEventListener() = delete;
	~KeyEventListener();

	static void RegisterEngineObject(CoreEngine* engine_);
	static bool GetKeyState(std::string keyname_);
	static void SetKeyState(SDL_KeyboardEvent* key);

private:
	static CoreEngine* engineInstance;
	static const char* keyname;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
};

#endif // !KEYEVENTLISTENER_H

