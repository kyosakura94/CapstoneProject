#ifndef GAMETEST_H
#define GAMETEST_H

//.. go to level up in solution (move around folder)
#include "..\Engine\Core\CoreEngine.h"
#include "Scene/StartScene.h"
#include "Scene/GameScene.h"

class GameTest : public GameInterface
{
public:
	GameTest();
	virtual ~GameTest();
	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
private:
	Scene* currentScene;
	int currentSceneNum;
	void BuildScene();

};

#endif // !GAMETEST_H



