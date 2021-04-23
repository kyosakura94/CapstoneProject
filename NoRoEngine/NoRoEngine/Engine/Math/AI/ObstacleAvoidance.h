#ifndef OBSTACLEAVOIDANCE_H
#define OBSTACLEAVOIDANCE_H

#include "Seek.h"
#include "../../Math/Ray.h"
#include "../../Math/CollisionHandler.h"
#include "../../Math/CollisionDetection.h"


class ObstacleAvoidance : public Seek
{
public:
	ObstacleAvoidance();
	ObstacleAvoidance(GameObject* character_, GameObject* target_);
	~ObstacleAvoidance();
	void setmaxAcceleration(float maxAcceleration_);
	SteeringOutput getSteering();

private:
	float avoidDistance; // distance to look ahead for a collision // (ie length of the collision ray) 
	float lookahead;
	GameObject* character;
	GameObject* target;
	SteeringOutput result;
	float maxAcceleration;
};

#endif


