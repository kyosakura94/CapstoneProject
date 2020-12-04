#include "EventListener.h"
#include "../Core/CoreEngine.h"

EventListener::~EventListener()
{

}

void EventListener::Update()
{
	const Uint8*  keystate = SDL_GetKeyboardState(NULL);
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		if (sdlEvent.type == SDL_QUIT)
		{
			CoreEngine::GetInstance()->Exit();
		}
		switch (sdlEvent.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			MouseEventListener::Update(sdlEvent, keystate);
			break;
		case SDL_KEYUP:
			KeyEventListener::SetKeyState(&sdlEvent.key);
			PrintKeyInfo(&sdlEvent.key);
			break;
		default:

			break;
		}
	}
}


void EventListener::PrintKeyInfo(SDL_KeyboardEvent* key) {

    if (key->type == SDL_KEYUP)
        printf("Release:- ");
    else
        printf("Press:- ");


    printf("Scancode: 0x%02X ", key->keysym.scancode);
    printf(", Name: %s \n", SDL_GetKeyName(key->keysym.sym));
}