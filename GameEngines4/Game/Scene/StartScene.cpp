#include "StartScene.h"

StartScene::StartScene() : Scene()
{

}

StartScene::~StartScene()
{

}

bool StartScene::OnCreate()
{
	Debug::Info("Create Start Scene", "StartScene.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCurrentScene(1);
	return true;
}

void StartScene::Update(const float deltaTime_)
{
}

void StartScene::Render()
{
}

void StartScene::Draw()
{
}
