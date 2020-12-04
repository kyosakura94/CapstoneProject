#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.h"
#include "../../Rendering/3D/Component.h"

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


