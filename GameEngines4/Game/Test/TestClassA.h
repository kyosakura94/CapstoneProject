#ifndef TESTCLASSA_H
#define TESTCLASSA_H

#include "../../Engine/Rendering/3D/Component.h"
#include "../../Engine/Math/AI/SteeringOutput.h"
#include <iostream>

class TestClassA :  public Component
{
public:
	TestClassA(std::string test_);
	virtual ~TestClassA();
	virtual bool OnCreate(GameObject* parent);
	virtual void Update(float deltaTime_);

private:
	GameObject * gameObject;
	SteeringOutput steering;
};


#endif // !TESTCLASSA_H


