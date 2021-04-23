#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H

#include "../../Engine/Animation/animation_sub/Animator.h"
#include "../../Engine/Rendering/3D/Component.h"
#include "../../Engine/Math/AI/SteeringOutput.h"
#include "../../Engine/Rendering/3D/GameObject.h"
#include "../../Engine/Event/KeyEventListener.h"
#include "../../Engine/Rendering/SceneGraph/SceneGraph.h"
#include <iostream>
#include "Player.h"

class PlayerMovement : public Component
{
public:

	PlayerMovement();
	virtual ~PlayerMovement();
	virtual bool OnCreate(GameObject* parent_);
	virtual void Update(float deltaTime_);
	virtual bool delayedUpdate();

	void characterMovement(const float deltatime_, bool inputs[4]);
private:

	Animator* currentAniComponent;
	Player* playerComponent;
	AudioSource* currentAudio;
	bool onMove = false;
	bool onRotate = false;
	float speed = 20.0f;
	void Move(vec3 _inputDirection, vec3 angularVelocity, const float time_);
	void Move(vec3 _inputDirection, vec3 angularVelocity,float angle, const float time_);
	GameObject* gameObject;
	SteeringOutput steering;
	float test;
	bool input[4];
	GameObject* target;
};

#endif
