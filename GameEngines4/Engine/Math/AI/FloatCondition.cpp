#include "FloatCondition.h"

FloatCondition::FloatCondition(std::string name_) : conditionName(""), result(false)
{
	conditionName = name_;
}
FloatCondition::~FloatCondition()
{

}

bool FloatCondition::Test()
{
	return result;
}

std::string FloatCondition::getName()
{
	return conditionName;
}

bool FloatCondition::setResult(float valueA, float valueB)
{
	if (valueA > valueB )
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}


