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
bool Window::OnCreate(std::string name_, int width_, int height_)
{
	if (SDL_Init(SDL_INIT_VIDEO < 0))
	{
		Debug::Fatal_error("SDL failed to innitate", "Window.cpp", __LINE__);
		//std::cout << "SDL failed to innitate" << std::endl;
		return false;
	}
	
	width = width_;
	height = height_;

	SetPreAttributes();

	//c_str() convert string to char
	window = SDL_CreateWindow(name_.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		SDL_WINDOW_OPENGL);

	if (!window)
	{
		Debug::Fatal_error("Window failed to create", "Window.cpp", __LINE__);
//		std::cout << "Window failed to create" << std::endl;
		return false;
	}

	context = SDL_GL_CreateContext(window);

	SetPostAttributes();

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		Debug::Fatal_error("Glew failed to initialize", "Window.cpp", __LINE__);
		//std::cout << "Glew failed to initialize" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	//resize window a: update width height. b: call this functoin again
	glViewport(0, 0, width, height);

	std::cout << "Graphic Cards:"<< glGetString(GL_VENDOR) << std::endl;
	std::cout << "Open Version:"<< glGetString(GL_VERSION) << std::endl;

	return true;
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

void Window::SetPreAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5 );
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
}
void Window::SetPostAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	glEnable(GL_MULTISAMPLE);
}

