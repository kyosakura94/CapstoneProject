#include "GameTest.h"

GameTest::GameTest() : GameInterface(), currentScene(nullptr), currentSceneNum(0)
{

}
GameTest::~GameTest()
{
	delete currentScene;
	currentScene = nullptr;

}

bool GameTest::OnCreate()
{
	ShaderHandler::getInstance()->CreateProgram("basicShader", "Engine/Shaders/VertexShader.glsl", "Engine/Shaders/FragmentShader.glsl");
	ShaderHandler::getInstance()->CreateProgram("spriteShader", "Engine/Shaders/SpriteVertShader.glsl", "Engine/Shaders/SpriteFragShader.glsl");
	ShaderHandler::getInstance()->CreateProgram("particleShader", "Engine/Shaders/ParticleVertexShader.glsl", "Engine/Shaders/ParticleFragmentShader.glsl");

	if (CoreEngine::GetInstance()->GetCurrentScene() == 0 )
	{
		currentScene = new StartScene();
		if (!currentScene->OnCreate())
		{
			//DEBUG
			Debug::Warning("Scene is not created ", "GameTest.cpp", __LINE__);

			return false;
		}
		//currentSceneNum = CoreEngine::GetInstance()->GetCurrentScene();

		return true;
	}
	Debug::Fatal_error("Engine SceneNumber is not zero", "GameTest.cpp", __LINE__);
	return false;
}

void GameTest::Update(const float deltaTime_)
{
	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentScene())
	{
		BuildScene();
	}
	currentScene->Update(deltaTime_);
}

void GameTest::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	//glClearColor(1.0f, 1.0f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentScene->Render();
	//currentScene->Draw();

	//Game's render
	SDL_GL_SwapWindow(CoreEngine::GetInstance()->getWindow()->GetWindow());
}

void GameTest::BuildScene()
{
	delete currentScene;
	currentScene = nullptr;

	switch (CoreEngine::GetInstance()->GetCurrentScene())
	{
	case 1:
		currentScene = new GameScene();
		break;
	default:
		//currentScene = new StartScene();
		break;
	}

	if (!currentScene->OnCreate())
	{
		//DEBUG
		Debug::Error("Cannot build scene!", "GameTest.cpp", __LINE__);
		CoreEngine::GetInstance()->Exit();
	}

	currentSceneNum = CoreEngine::GetInstance()->GetCurrentScene();
}
