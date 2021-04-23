#include "ObstacleAvoidance.h"

SteeringOutput ObstacleAvoidance::getSteering()
{
	vec3 ray = character->GetVelocity();
	ray = normalize(ray);
	ray *= lookahead;

	Ray e = Ray(character->GetPosition(), ray);

	if (e.isColliding(&target->GetBoundingBox()))
	{
		//vec3 targetq = collision.position + collision.normal * avoidDistance;
		result.setLinear(normalize(target->GetPosition() - character->GetPosition()));
		result.setLinear(result.getLinear() *= maxAcceleration);
		result.setAngular(0);

		return result;
	}
	else
	{
		return result;
	}

}
