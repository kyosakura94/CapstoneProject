#pragma once
#include "../../Engine/Rendering/3D/Component.h"
#include "../../Engine/Rendering/3D/GameObject.h"
#include "../../Engine/Rendering/SceneGraph/SceneGraph.h"
#include <string>

class Player : public Component
{

public:
	Player();
	virtual ~Player();
	virtual bool OnCreate(GameObject* parent_);
	virtual void Update(float deltaTime_);
	virtual bool delayedUpdate();
	void collideRocket(bool hit_, string tag_);
	bool gethitbyRocket() { return hitbyRocket; };
	bool isDeath() { return isDie; };
	void UISetUP(string ds, string dk_);
private:
	bool appleCollect = false;
	bool hitbyRocket = false;
	vec3 originalPosition;
	int lifeCount = 3;
	string hittag;
	bool isDie = false;
	bool isFirsttime = true;
	AudioSource* backroundSound;
};

