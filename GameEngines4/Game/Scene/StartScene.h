#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "../../Engine/Core/CoreEngine.h"
class StartScene : public Scene
{
public:
	StartScene();
	virtual ~StartScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void Draw();
	virtual void CreatePlayer(vec3 pos, string modelName, string tag);
	virtual void CreatePlayer(string json_);
};
#endif // !STARTSCENE_H



