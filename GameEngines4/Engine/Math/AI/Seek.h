#ifndef SEEK_H
#define SEEK_H

#include "../../Rendering/3D/Model.h"
#include "SteeringOutput.h"
#include "../../Rendering/3D/GameObject.h"
using namespace glm;
using namespace std;

class Seek
{
public:
	Seek();
	Seek(GameObject* character_, GameObject* target_);
	~Seek();
	void setmaxAcceleration(float maxAcceleration_);
	SteeringOutput getSteering();

private:
	GameObject* character;
	GameObject* target;
	SteeringOutput result;
	float maxAcceleration;
};


#endif // !SEEK_H


