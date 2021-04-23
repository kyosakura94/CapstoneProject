#include "Seek.h"
Seek::Seek()
{
}

Seek::Seek(GameObject* character_, GameObject* target_)
{
	character = character_;
	target = target_;
}

Seek::~Seek()
{
}

void Seek::setmaxAcceleration(float maxAcceleration_)
{
	maxAcceleration = maxAcceleration_;
}



SteeringOutput Seek::getSteering()
{
	result.setLinear(normalize(target->GetPosition() - character->GetPosition()));
	result.setLinear(result.getLinear() *= maxAcceleration);
	result.setAngular(0);

	return result;
}



