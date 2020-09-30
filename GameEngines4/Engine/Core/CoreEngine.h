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

	void SetGameInterface(GameInterface * gameInterface_);

	void SetCurrentScene(int sceneNum_);
	int GetCurrentScene();
	glm::vec2 GetWindowSize() const;

	void SetCamera(Camera* camera_);
	Camera* GetCamera() const;

	void NotifyOfMousePressed(vec2 mouse_);
	void NotifyOfMouseReleased(vec2 mouse_, int buttonType_);
	void NotifyOfMouseMove(vec2 mouse_);
	void NotifyOfMouseScroll(int y_);

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

	Timer timer;
	unsigned int fps;

	GameInterface* gameInterface;

	int currentSceneNum;
	Camera *camera;
	
};
#endif // !1


