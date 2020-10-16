#ifndef COLLISIONAVOIDANCE_H
#define COLLISIONAVOIDANCE_H

#include "../../Rendering/3D/Model.h"
#include "SteeringOutput.h"
#include "../../Rendering/3D/GameObject.h"

class CollisionAvoidance
{
public:
	CollisionAvoidance();
	CollisionAvoidance(GameObject* character_, vector<GameObject*> target_);
	CollisionAvoidance(GameObject* character_, GameObject* movetarget_, vector<GameObject*> target_);
	~CollisionAvoidance();
	void setmaxAcceleration(float maxAcceleration_);
	void setRadius(float radius_);
	SteeringOutput getSteering();

private:
	GameObject* character;
	GameObject* movetarget;
	SteeringOutput result;
	float maxAcceleration;
	float radius;
	vector<GameObject*> target;
};

#endif // !COLLISIONAVOIDANCE_H



