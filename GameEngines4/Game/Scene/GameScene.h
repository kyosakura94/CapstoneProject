#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../Engine/Core/CoreEngine.h"
#include <glm/gtx/string_cast.hpp>
#include "../../Game/Test/TestClassA.h"
#include "../../Engine/Rendering/2D/GuiImageComponent.h"
#include "../../Engine/Math/Quaternion.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void Draw();
private:

};
#endif