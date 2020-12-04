#ifndef TRANSITION_H
#define TRANSITION_H

#include "Condition.h"
#include <glm/glm.hpp>
#include "Actions.h"
#include <vector>

using namespace std;
using namespace glm;
class State;
class Transition
{
public:

	Transition();

	Transition(int i);
	~Transition();

	bool isTriggered();
	State* getTargetState();
	Condition* getTransitionCondition() { return condition; };
	Actions	getAction();

	void setTargetState(State* target_) { targetState = target_; };
	void setTransitionCondition(Condition* condition_) { condition = condition_; };


	//vector<Actions*> getActions();
private:
	State * targetState;
	Actions action;
	Condition * condition;

	//vector<Actions*> actions;
};


#endif // !TRANSITION_H

