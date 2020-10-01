#include "TestClassA.h"

TestClassA::TestClassA() : Component()
{

}

TestClassA::~TestClassA()
{

}

bool TestClassA::OnCreate(GameObject *parent_) 
{
	parent = parent_;
	return true;
}

void TestClassA::Update(float deltaTime_)
{
	std::cout << "Update component A" << std::endl;
}

