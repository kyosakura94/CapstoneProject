#include "Condition.h"

unique_ptr<Condition> Condition::conditionInstance = nullptr;

Condition::Condition () : check(false), inside(false), outside(false)
{
}

Condition::~Condition()
{
}

Condition* Condition::GetInstance()
{
	if (conditionInstance.get() == nullptr)
	{

		conditionInstance.reset(new Condition);
	}
	return conditionInstance.get();
}

bool Condition::Test()
{
	//if (inside == true)
	//{
	//	return check;
	//}

	//if (outside == true)
	//{
	//	return !check;
	//}

	return check;
}

bool Condition::collide()
{
	return false;
}

bool Condition::noCollide()
{
	return false;
}
