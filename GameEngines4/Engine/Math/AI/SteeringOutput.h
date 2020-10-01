#ifndef STEERINGOUTPUT_H
#define STEERINGOUTPUT_H

#include "../../Rendering/3D/Model.h"

using namespace glm;
using namespace std;
class SteeringOutput
{
public:
	SteeringOutput();
	~SteeringOutput();

	vec3 getLinear();
	float getAngular();

	void setLinear(vec3 linear_);
	void setAngular(float angular_);

private:
	vec3 linear;
	float angular;
};


#endif // !STEERINGOUTPUT_H





