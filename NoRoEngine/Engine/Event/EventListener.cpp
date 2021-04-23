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
				//printf("Mouse pressed \n");
				//MouseEventListener::setMouseClick(true);
				//break;

			case SDL_MOUSEBUTTONUP:	
				//printf("Mouse up \n");
				//MouseEventListener::setMouseClick(false);
				//break;

			case SDL_MOUSEMOTION:

			case SDL_MOUSEWHEEL:
				MouseEventListener::Update(sdlEvent, keystate);
				break;

			case SDL_KEYUP:

				KeyEventListener::SetKeyState(&sdlEvent.key);
				//PrintKeyInfo(&sdlEvent.key);
				//printf("Name: %s \n", SDL_GetKeyName(sdlEvent.key.keysym.sym));
				//printf("Released ", &sdlEvent.key.keysym.scancode);
			
				break;			
			
			case SDL_KEYDOWN:

				KeyEventListener::SetKeyState(&sdlEvent.key);
				//PrintKeyInfo(&sdlEvent.key);

				//printf("Name: %s \n", SDL_GetKeyName(sdlEvent.key.keysym.sym));
				//printf("Pressed %d", &sdlEvent.key.keysym.scancode);
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