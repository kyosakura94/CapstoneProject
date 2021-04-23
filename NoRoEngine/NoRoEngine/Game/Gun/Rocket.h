#pragma once
#include "../../Engine/Rendering/3D/Component.h"
#include "../../Engine/Animation/animation_sub/Animator.h"
#include "../../Engine/Math/AI/SteeringOutput.h"
#include "../../Engine/Rendering/3D/GameObject.h"
#include "../../Engine/Event/KeyEventListener.h"
#include "../../Engine/Rendering/SceneGraph/SceneGraph.h"
#include "../Player/Player.h"
#include <iostream>

class Rocket : public Component
{
public:
	Rocket();
	virtual ~Rocket();
	virtual bool OnCreate(GameObject* parent_);
	virtual void Update(float deltaTime_);
	virtual bool delayedUpdate();
	void TriggerCollider();
	void MoveForward(float time_);

private:
	GameObject* player;
	int index_ = 0;
	Player *playerComponent;
};

