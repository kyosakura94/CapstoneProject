#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "Renderer.h"
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <string> 
#include <iostream>


class OpenGLRenderer : public Renderer
{
public:

    OpenGLRenderer();
    virtual ~OpenGLRenderer();
    virtual SDL_Window* CreateWindow(const std::string name_, int width_, int height_);
    virtual	bool OnCreate();
    virtual void OnDestroy();
    virtual void Render();

private:
    void SetPreAttributes();
    void SetPostAttributes();
    int width, height;
    SDL_Window * window;
    SDL_GLContext context;
};

#endif // !OPENGLRENDERER_H



