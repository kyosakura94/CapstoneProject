#include "TestClassA.h"

TestClassA::TestClassA(std::string test_) : Component()
{
	std::cout << test_ << std::endl;
}

TestClassA::~TestClassA()
{

}

bool TestClassA::OnCreate(GameObject * parent_) 
{
	
	parent = parent_;
	return true;
}

void TestClassA::Update(float deltaTime_)
{

	std::cout << "Update component A"<< std::endl;
}

bool TestClassA::delayedUpdate()
{
	return false;
}



