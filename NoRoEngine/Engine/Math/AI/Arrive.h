#ifndef ARRIVE_H
#define ARRIVE_H

#include "../../Rendering/3D/Model.h"
#include "SteeringOutput.h"
#include "../../Rendering/3D/GameObject.h"

using namespace glm;
using namespace std;
class Arrive
{
public:
	Arrive();
	Arrive(GameObject * character_, GameObject * target_);
	~Arrive();
	void setmaxAcceleration(float maxAcceleration_);
	void settargetRadius(float maxSpeed_);
	void setslowRadius(float targetRadius_);
	void setmaxSpeed(float slowRadius_);
	SteeringOutput getSteering();

private:
	GameObject* character;
	GameObject* target;
	SteeringOutput result;

	float maxAcceleration;
	float maxSpeed;
	float targetRadius;
	float slowRadius;
	float timeToTarget = 0.1;
};


#endif // !ARRIVE_H


