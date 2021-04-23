#pragma once
#include "../../Engine/Rendering/3D/Component.h"
#include "../../Engine/Animation/animation_sub/Animator.h"
#include "../../Engine/Math/AI/SteeringOutput.h"
#include "../../Engine/Rendering/3D/GameObject.h"
#include "../../Engine/Event/KeyEventListener.h"
#include "../../Engine/Rendering/SceneGraph/SceneGraph.h"
#include "../../Engine/FX/Partice/ParticleEmitter.h"
#include <iostream>
#include "../Player/Player.h"

class Shoot : public Component
{
public:
	Shoot();
	virtual ~Shoot();
	virtual bool OnCreate(GameObject* parent_);
	virtual void Update(float deltaTime_);
	virtual bool delayedUpdate();
	void fireTrigger();
	void setType(int type_) { type = type_; };
private:
	Model* RocketModel;
	GameObject* player_;
	Player * playerComponent;
	int index_ = 0;
	float timeRate;
	bool shoot = false;
	vec3 offset = vec3(-0.5f, 1.8f, 0);
	int type = 0;
};

