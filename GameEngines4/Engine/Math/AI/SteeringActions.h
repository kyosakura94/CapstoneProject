#ifndef STEERINGACTIONS_H
#define STEERINGACTIONS_H
 
#include "Actions.h"
class SteeringActions : public Actions
{
public:
	SteeringActions(GameObject* gameobject_);
	virtual ~SteeringActions();
	virtual void DoSomething(const float deltaTime_);

private:
	CollisionAvoidance* avoidance;
	std::vector<GameObject*> targetList;
	GameObject* gameObject;
};
#endif // !STEERINGACTIONS_H



