#ifndef STATE_H
#define STATE_H

#include <glm/glm.hpp>
#include <vector>
#include "Transition.h"
#include "../../Rendering/3D/GameObject.h"
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

	Actions * getActions();
	Actions * getEntryActions();
	Actions * getExitActions();
	void setTransitions(Transition* transition_);
	void setGameObject(GameObject * gameObject_);
	vector<Transition*> getTransitions() { return transitions; }
	void setAction(Actions * action_, int i);

private:
	Actions* mainAction;
	Actions* entryAction;
	Actions* exitAction;
	GameObject* gameObject;
	vector<Transition*> transitions;
};

#endif // !STATE_H


