#include "StateMachine.h"

StateMachine::StateMachine()
{
	State* idleState = new State(1);
	State* attackState = new State(2);

	Transition* idleTransition = new Transition();
	idleTransition->setTargetState(attackState);	
	//idleTransition->setTransitionCondition(new Condition());

	Transition* attackTransition = new Transition();
	attackTransition->setTargetState(idleState);
	//attackTransition->setTransitionCondition(new TestCondition());

	
	idleState->setTransitions(idleTransition);
	attackState->setTransitions(attackTransition);

	initialState = idleState;

	currentState = initialState;
}

StateMachine::~StateMachine()
{
}

bool StateMachine::OnCreate(GameObject* parent)
{
	gameObject = parent;
	return true;
}


bool StateMachine::delayedUpdate()
{
	return false;
}

void StateMachine::Update(float deltaTime)
{
	Transition * trigger = nullptr;

	for (auto transition : currentState->getTransitions())
	{
		if (transition->isTriggered())
		{
			trigger = transition;
			break;
		}
	}

	if (trigger != nullptr)
	{
		State* targetState = trigger->getTargetState();

		Actions actions = currentState->getExitActions();

		Actions entryAction = targetState->getEntryActions();

		Actions transitionAction = trigger->getAction();;

		actions += transitionAction;

		actions += entryAction;
		currentState = targetState;
		currentState->getActions();
	}
	else
		currentState->getActions();

}
