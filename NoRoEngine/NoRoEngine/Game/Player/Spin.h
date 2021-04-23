#pragma once
#include "../../Engine/Rendering/3D/Component.h"
#include "../../Engine/Rendering/3D/GameObject.h"
#include "../../Engine/Animation/animation_sub/Animator.h"

class Spin : public Component
{
public:
	Spin();
	Spin(float spinRate_);
	virtual ~Spin();
	virtual bool OnCreate(GameObject* parent_);
	virtual void Update(float deltaTime_);
	virtual bool delayedUpdate();
private:
	float spinRate; 
	GameObject* gameObject;

};

