#include "Window.h"
#include "Debug.h"

//Context consider as a door to communicate with GPU
Window::Window() : window(nullptr), width(0), height(0)
{
}


Window::~Window()
{
	OnDestroy();
}
//Create out to in
bool Window::OnCreate(std::string name_, int width_, int height_, Renderer * renderer_)
{
	width = width_;
	height = height_;

	if (SDL_Init(SDL_INIT_VIDEO < 0))
	{
		Debug::Fatal_error("SDL failed to innitate", "Window.cpp", __LINE__);
		//std::cout << "SDL failed to innitate" << std::endl;
		return false;
	}
	
	window = renderer_->CreateWindow(name_, width_, height_);
	
	if (window != nullptr)
	{
		return true;
	}
	else
	{
		Debug::Fatal_error("Window failed to create", "Window.cpp", __LINE__);
		return false;
	}

}

//destroy in to out
void Window::OnDestroy() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

	window = nullptr;
}

int Window::GetHeight() {

	return height;
}

int Window::GetWidth() {

	return width;
}

SDL_Window * Window::GetWindow() const
{
	return window;
}



