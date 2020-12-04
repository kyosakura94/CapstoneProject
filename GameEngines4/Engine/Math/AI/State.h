#ifndef STATE_H
#define STATE_H

#include <glm/glm.hpp>
#include <vector>
#include "Transition.h"

using namespace std;
using namespace glm;

enum STATE
{

};

class State
{
public:

	State();
	State(int i);
	~State();

	Actions getActions();
	Actions getEntryActions();
	Actions getExitActions();
	void setTransitions(Transition* transition_);

	vector<Transition*> getTransitions() { return transitions; }

private:
	vector<Transition*> transitions;
};

#endif // !STATE_H


