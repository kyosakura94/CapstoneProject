#include "SteeringOutput.h"
SteeringOutput::SteeringOutput()
{
}

SteeringOutput::~SteeringOutput()
{
}

vec3 SteeringOutput::getLinear()
{
	return linear;
}

float SteeringOutput::getAngular()
{
	return angular;
}

void SteeringOutput::setLinear(vec3 linear_)
{
	linear = linear_;
}

void SteeringOutput::setAngular(float angular_)
{
	angular = angular_;
}
