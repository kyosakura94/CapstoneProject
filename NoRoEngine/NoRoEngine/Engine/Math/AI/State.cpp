#include "State.h"

State::State()
{
}

State::State(int i)
{
	switch (i)
	{
	case 1:
		cout << "IDLE STATE" << endl;
		break;
	case 2:
		cout << "ATTACK STATE" << endl;
		break;
	default:
		break;
	}
}

State::~State()
{
}

Actions * State::getActions()
{
	return mainAction;
}

Actions * State::getEntryActions()
{
	return entryAction;
}


Actions * State::getExitActions()
{
	return exitAction;
}

void State::setTransitions(Transition* transition_)
{
	if (transition_ != nullptr)
	{
		transitions.push_back(transition_);
	}
}

void State::setGameObject(GameObject* gameObject_)
{
	gameObject = gameObject_;
}

void State::setAction(Actions* action_, int i)
{
	switch (i)
	{
	case 0:
		entryAction = action_;
		break;
	case 1:
		mainAction = action_;
		break;
	case 2:
		exitAction = action_;
		break;
	default:
		break;
	}
	
}

