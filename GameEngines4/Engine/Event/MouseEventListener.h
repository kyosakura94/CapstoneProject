#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>

class CoreEngine;
using namespace glm;
class MouseEventListener
{
public:
	MouseEventListener(const MouseEventListener&) = delete;
	MouseEventListener(MouseEventListener&&) = delete;
	MouseEventListener& operator = (const MouseEventListener&) = delete;
	MouseEventListener& operator = (MouseEventListener&&) = delete;

	MouseEventListener() = delete;

	static void RegisterEngineObject(CoreEngine* engine_);
	static void Update(SDL_Event e_);

	~MouseEventListener();

	static void NotifyOfMousePressed(int buttonType_);
	static void NotifyOfMouseReleased(int butoonType_);
	static void NotifyOfMouseMove();
	static void NotifyOfMouseScroll(int y_);

	static vec2 GetMousePosition();
	static vec2 GetPreviousMousePosition();
	static vec2 GetMouseOffset();
private:
	static CoreEngine* engineInstance;
	static vec2 curMouse, preMouse;
	static bool firstUpdate;
	static void UpdateMousePosition();
};

#endif // !MOUSEEVENTLISTENER

