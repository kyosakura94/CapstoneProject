#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine() : window(nullptr), isRunning(false), fps(120), 
							gameInterface(nullptr), currentSceneNum(0), camera(nullptr)
{

}


CoreEngine::~CoreEngine()
{

}

bool CoreEngine::OnCreate(std::string name_, int width_, int height_)
{
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);
	window = new Window();
	if (!window->OnCreate(name_, width_, height_))
	{

		Debug::Fatal_error("Window failed to innitate", "CoreEngine.cpp", __LINE__);
		//std::cout << "Window failed to innitate" << std::endl;
		return isRunning = false;
	}
	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);

	MouseEventListener::RegisterEngineObject(this);

	//ShaderHandler::getInstance()->CreateProgram("colorShader", "Engine/Shaders/ColorVertexShader.glsl", "Engine/Shaders/ColorFragmentShader.glsl");
	
	ShaderHandler::getInstance()->CreateProgram("basicShader", "Engine/Shaders/VertexShader.glsl", "Engine/Shaders/FragmentShader.glsl");
	
	if (gameInterface) //Gameinterface is not equal to != nullptr
	{
		if (!gameInterface->OnCreate())
		{
			//Debug
			Debug::Fatal_error("Game interface failed to innitate", "CoreEngine.cpp", __LINE__);
			return isRunning = false;
		}
	}


	timer.Start();
	Debug::Info("Everything was created fine", "CoreEngine.cpp", __LINE__);
	return isRunning = true;
}

void CoreEngine::Run()
{
	while (isRunning)
	{
		timer.UpdateFrameTicks();
		EventListener::Update();

		Update(timer.GetDeltaTime());
		Render();

		SDL_Delay(timer.GetSleepTime(fps));
	}

	if (!isRunning)
	{
		OnDestroy();
	}
}

bool CoreEngine::GetIsRunning() const
{
	return isRunning;
}

CoreEngine * CoreEngine::GetInstance()
{
	if (engineInstance.get() == nullptr)
	{
		engineInstance.reset(new CoreEngine);
	}
	return engineInstance.get();
}

void CoreEngine::SetGameInterface(GameInterface * gameInterface_)
{
	gameInterface = gameInterface_;
}

void CoreEngine::SetCurrentScene(int sceneNum_)
{
	currentSceneNum = sceneNum_;
}

int CoreEngine::GetCurrentScene()
{
	return currentSceneNum;
}

glm::vec2 CoreEngine::GetWindowSize() const
{
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

void CoreEngine::SetCamera(Camera * camera_)
{
	camera = camera_;
}

Camera * CoreEngine::GetCamera() const
{
	return camera;
}

void CoreEngine::NotifyOfMousePressed(vec2 mouse_)
{

}

void CoreEngine::NotifyOfMouseReleased(vec2 mouse_, int buttonType_)
{
	CollisionHandler::GetInstance()->MouseUpdate(mouse_, buttonType_);
}

void CoreEngine::NotifyOfMouseMove(vec2 mouse_)
{
	if (camera)
	{
		camera->ProcessMouseMovement(MouseEventListener::GetMouseOffset());
	}
}

void CoreEngine::NotifyOfMouseScroll(int y_)
{
	if (camera)
	{
		camera->ProcessMoouseScroll(y_);
	}
}

void CoreEngine::Exit()
{
	isRunning = false;
}

void CoreEngine::OnDestroy()
{
	ShaderHandler::getInstance()->OnDestroy();
	TextureHandler::getInstance()->OnDestroy();
	SceneGraph::GetInstance()->OnDestroy();
	CollisionHandler::GetInstance()->OnDestroy();

	delete camera;
	camera = nullptr;
	delete gameInterface;
	gameInterface = nullptr;

	delete window;
	window = nullptr;

	SDL_Quit();
	exit(0);
}

void CoreEngine::Update(const float timeDelta_)
{
	if (gameInterface)
	{
		gameInterface->Update(timeDelta_);
	}
}

void CoreEngine::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	//glClearColor(1.0f, 1.0f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gameInterface)
	{
		gameInterface->Render();
	}
	//Game's render
	SDL_GL_SwapWindow(window->GetWindow());
}
