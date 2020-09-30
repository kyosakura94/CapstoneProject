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
	currentScene->Render();
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
		currentScene = new StartScene();
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
