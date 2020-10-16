#include "CollisionAvoidance.h"

CollisionAvoidance::CollisionAvoidance()
{
}

CollisionAvoidance::CollisionAvoidance(GameObject* character_, vector<GameObject*> target_)
{
	character = character_;
	target = target_;
}

CollisionAvoidance::CollisionAvoidance(GameObject* character_, GameObject* movetarget_, vector<GameObject*> target_)
{
	character = character_;
	target = target_;
	movetarget = movetarget_;
}

CollisionAvoidance::~CollisionAvoidance()
{

}

void CollisionAvoidance::setmaxAcceleration(float maxAcceleration_)
{
	maxAcceleration = maxAcceleration_;
}

void CollisionAvoidance::setRadius(float radius_)
{
	radius = radius_;
}


SteeringOutput CollisionAvoidance::getSteering()
{
	float shortestTime = INFINITY;
	vec3 relativePos;

	GameObject* firstTarget = nullptr;
	float firstMinSeparation = 0;
	float firstDistance = 0;
	vec3 firstRelativePos;
	vec3 firstRelativeVel;

	for (size_t i = 0; i < target.size(); i++)
	{
	//Calculate the time to collision.
		relativePos = target[i]->GetPosition() - character->GetPosition();
		vec3 relativeVel = target[i]->GetVelocity() - character->GetVelocity();
		float relativeSpeed = glm::length(relativeVel);

		float timeToCollision = dot(relativePos, relativeVel) / (relativeSpeed * relativeSpeed);

		//Check if it is going to be a collision at all

		float distance = glm::length(relativePos);
		float minSeparation = distance - relativeSpeed * timeToCollision;

		if (minSeparation > 2 * radius)
		{
			continue;
		}

		if (timeToCollision > 0 && timeToCollision < shortestTime)
		{
			shortestTime = timeToCollision;
			firstTarget = target[i];
			firstMinSeparation = minSeparation;
			firstDistance = distance;
			firstRelativePos = relativePos;
			firstRelativeVel = relativeVel;
		}
		
	}

	if (firstTarget == nullptr)
	{
		result.setLinear(normalize(movetarget->GetPosition() - character->GetPosition()));
		result.setLinear(result.getLinear() *= maxAcceleration);
		result.setAngular(0);
		return result;
	}

	//colliding, then do the steering based on current position.
	if (firstMinSeparation <= 0 && firstDistance < 2 * radius) 
	{
		relativePos = firstTarget->GetPosition() - character->GetPosition();
	}
	else
	{
		relativePos = firstRelativePos + firstRelativeVel * shortestTime;
	}
	relativePos = normalize(relativePos);


	result.setLinear(relativePos * maxAcceleration);
	result.setAngular(0);

	return result;
}
