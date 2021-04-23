#ifndef RENDERER_H
#define RENDERER_H
#include <SDL.h>
#include <string>

enum class RendererType
{
	NONE,
	OPENGL,
	VULKAN,
	DIRECTX11,
	DIRECTX12
};

class Renderer
{
public:
	Renderer(): renderer(nullptr), rendererType(RendererType::NONE) {}
	virtual ~Renderer() {}
	virtual SDL_Window* CreateWindow(const std::string name_, int width, int height) = 0;
	virtual	bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Render() = 0;

	RendererType getRendererType() { return rendererType; };
	void setRendererType(RendererType rendererType_) { rendererType = rendererType_; };

private:
	Renderer* renderer;
	RendererType rendererType;
};


#endif // !RENDERER_H


