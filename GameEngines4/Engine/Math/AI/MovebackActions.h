#ifndef MOVEBACKACTIONS_H
#define MOVEBACKACTIONS_H

#include "Actions.h"
class MovebackActions : public Actions
{
public:
	MovebackActions(GameObject* gameobject_);
	virtual ~MovebackActions();
	virtual void DoSomething(const float deltaTime_);

private:
	CollisionAvoidance* avoidance;
	std::vector<GameObject*> targetList;
	GameObject* gameObject;
};
 
#endif // !MOVEBACKACTIONS_H



