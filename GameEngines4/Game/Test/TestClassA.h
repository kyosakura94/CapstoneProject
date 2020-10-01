#ifndef TESTCLASSA_H
#define TESTCLASSA_H

#include "../../Engine/Rendering/3D/Component.h"
#include <iostream>

class TestClassA :  public Component
{
public:
	TestClassA();
	virtual ~TestClassA();
	virtual bool OnCreate(GameObject* parent);
	virtual void Update(float deltaTime_);
};


#endif // !TESTCLASSA_H


