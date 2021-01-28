#ifndef DONOTHINGACTIONS_H
#define DONOTHINGACTIONS_H

#include "Actions.h"

class DoNothingActions : public Actions
{
public:

	DoNothingActions(GameObject* gameobject_);
	virtual ~DoNothingActions();
	virtual void DoSomething(const float deltaTime_);

private:
	CollisionAvoidance* avoidance;
	std::vector<GameObject*> targetList;
	GameObject* gameObject;
};




#endif // !DONOTHINGACTIONS_H



