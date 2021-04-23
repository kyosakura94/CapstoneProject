#ifndef COREENGINE_H
#define COREENGINE_H

#include "Window.h"
#include "Timer.h"
#include "Debug.h"
#include "GameInterface.h"
#include "Scene.h"
#include "../Rendering/3D/GameObject.h"
#include <memory>
#include "../Graphics/ShaderHandler.h"
#include "../Camera/Camera.h"
#include "../Graphics/TextureHandler.h"
#include "../Event/EventListener.h"
#include "../Rendering/SceneGraph/SceneGraph.h"
#include "../FX/Audio/AudioHandler.h"
#include "../Rendering/Renderer/Renderer.h"
#include "../Rendering/Renderer/OpenGLRenderer.h"
#include "../Network/Server.h"
#include <thread>
#include <future>

class CoreEngine
{
public:
	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator = (const CoreEngine&) = delete;
	CoreEngine& operator = (CoreEngine&&) = delete;


	bool OnCreate(std::string name_, int width_, int height_);
	void Run();
	bool GetIsRunning() const;

	static CoreEngine *GetInstance();

	void SetGameInterface(GameInterface * gameInterface_, RendererType rendererType_ = RendererType::OPENGL);

	void SetCurrentScene(int sceneNum_);
	void SetRunAgain(bool r_);
	int GetCurrentScene();
	bool GetRunAgain() { return runAgain;};
	glm::vec2 GetWindowSize() const;

	void SetCamera(Camera* camera_);
	Camera* GetCamera() const;

	void NotifyOfMousePressed(vec2 mouse_);
	void NotifyOfMouseReleased(vec2 mouse_, int buttonType_);
	void NotifyOfMouseMove(vec2 mouse_);
	void NotifyOfMouseScroll(int y_);
	void NotifyOfKeyPress();


	Renderer* getRenderer() { return renderer; }
	RendererType getRendererType() { return rendererType; }
	Window* getWindow() { return window; };


	void Exit();
private:
	CoreEngine();
	~CoreEngine();
	static std::unique_ptr<CoreEngine> engineInstance;
	friend std::default_delete<CoreEngine>;

	void OnDestroy();
	void Update(const float timeDelta_);
	void Render();

	Window *window; 
	bool isRunning;
	bool runAgain = false;

	Timer timer;
	unsigned int fps;

	GameInterface* gameInterface;

	int currentSceneNum;
	Camera *camera;

	Renderer* renderer;
	RendererType rendererType;
	
};
#endif // !1


