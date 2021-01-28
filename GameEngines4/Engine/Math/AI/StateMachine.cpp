#include "StateMachine.h"

StateMachine::StateMachine()
{

}

StateMachine::~StateMachine()
{
}

bool StateMachine::OnCreate(GameObject* parent)
{
	gameObject = parent;

	State* idleState = new State(1);
	State* attackState = new State(2);

	Transition* idleTransition = new Transition();
	idleTransition->setTargetState(attackState);
	FloatCondition* idleCondition = new FloatCondition("floatIdle");
	idleTransition->setTransitionCondition(idleCondition);

	Transition* attackTransition = new Transition();
	attackTransition->setTargetState(idleState);
	FloatCondition* attackCondition = new FloatCondition("floatAttack");
	attackTransition->setTransitionCondition(attackCondition);

	ConditionManager::GetInstance()->addCondition(idleCondition);
	ConditionManager::GetInstance()->addCondition(attackCondition);

	idleState->setTransitions(idleTransition);
	attackState->setTransitions(attackTransition);

	initialState = idleState;

	currentState = initialState;

	idleState->setAction(new MovebackActions(gameObject), 1);
	idleState->setAction(new DoNothingActions(gameObject), 2);


	attackState->setAction(new SteeringActions(gameObject), 1);
	attackState->setAction(new DoNothingActions(gameObject), 2);

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

		currentState->getExitActions()->DoSomething(deltaTime);

		//Actions entryAction = targetState->getEntryActions();

		//Actions transitionAction = trigger->getAction();

		//actions += transitionAction;

		//actions += entryAction;

		currentState = targetState;

		currentState->getActions()->DoSomething(deltaTime);
	}
	else
		currentState->getActions()->DoSomething(deltaTime);

}
