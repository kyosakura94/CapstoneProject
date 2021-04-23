#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.h"
#include "FloatCondition.h"
#include "../../Rendering/3D/Component.h"
#include "ConditionManager.h"

#include "DoNothingActions.h"
#include "MovebackActions.h"
#include "SteeringActions.h"

class GameObject;
class StateMachine : public Component
{

public:
	StateMachine();
	virtual ~StateMachine();
	virtual bool OnCreate(GameObject* parent);
	virtual void Update(float deltaTime);
	virtual bool delayedUpdate();

private:
	GameObject* gameObject;
	State* initialState;
	State* currentState;
};

#endif // !STATEMACHINE_H


