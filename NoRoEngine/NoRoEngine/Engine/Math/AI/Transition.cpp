#include "Transition.h"
#include "State.h"

Transition::Transition()
{
	
}

Transition::Transition(int i)
{
	switch (i)
	{
	case 1:
		targetState = new State(2);
		break;
	case 2:
		targetState = new State(1);
	default:
		break;
	}
}

Transition::~Transition()
{
}

bool Transition::isTriggered()
{
	return condition->Test();
}

Actions * Transition::getAction()
{
	//action = Actions(3);
	return action;
}


State* Transition::getTargetState()
{
	return targetState;
}
