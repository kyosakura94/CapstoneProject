#include "Arrive.h"

Arrive::Arrive()
{
}

Arrive::Arrive(GameObject* character_, GameObject* target_)
{
	character = character_;
	target = target_;
}

Arrive::~Arrive()
{
}

void Arrive::setmaxAcceleration(float maxAcceleration_)
{
	maxAcceleration = maxAcceleration_;
}

void Arrive::settargetRadius(float maxSpeed_)
{
	maxSpeed = maxSpeed_;
}

void Arrive::setslowRadius(float targetRadius_)
{
	targetRadius = targetRadius_;
}

void Arrive::setmaxSpeed(float slowRadius_)
{
	slowRadius = slowRadius_;
}

SteeringOutput Arrive::getSteering()
{
	vec3 targetVelocity;
	float targetSpeed;
	vec3 direction = target->GetPosition() - character->GetPosition();
	float distance = direction.length();

	if (distance < targetRadius) 
	{
		return result;
	}

	if (distance > slowRadius)
	{
		targetSpeed = maxSpeed;
	}
	else
	{
		targetSpeed = maxSpeed * distance / slowRadius;
	}

	targetVelocity = direction;
	targetVelocity = normalize(targetVelocity);
	targetVelocity *= targetSpeed;

	result.setLinear(targetVelocity - character->GetVelocity());
	result.setLinear(result.getLinear() /= timeToTarget);

	if (result.getLinear().length() > maxAcceleration) 
	{
		result.setLinear(normalize(result.getLinear()));
		result.setLinear(result.getLinear() *= maxAcceleration);

	} 
	result.setAngular(0);

	return result;
}
