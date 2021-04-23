#ifndef KINEMATIC_H
#define KINEMATIC_H

#include "../../Rendering/3D/Model.h"

using namespace glm;
using namespace std;
class Kinematic
{
public:
	Kinematic();
	~Kinematic();
	void Update();

private:
	vec3 position;
	float orientation;
	vec3 velocity;
	float rotation;

};


#endif // !KINEMATIC_H


