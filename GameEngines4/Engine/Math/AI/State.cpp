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

Actions State::getActions()
{
	return Actions(2);
}

Actions State::getEntryActions()
{
	return Actions(0);
}


Actions State::getExitActions()
{
	return Actions(1);
}

void State::setTransitions(Transition* transition_)
{
	if (transition_ != nullptr)
	{
		transitions.push_back(transition_);
	}
}

