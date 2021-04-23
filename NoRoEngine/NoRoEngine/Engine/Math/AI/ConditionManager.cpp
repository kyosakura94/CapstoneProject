#include "ConditionManager.h"


unique_ptr<ConditionManager> ConditionManager::conditionInstance = nullptr;

ConditionManager* ConditionManager::GetInstance()
{
	if (conditionInstance.get() == nullptr)
	{

		conditionInstance.reset(new ConditionManager);
	}
	return conditionInstance.get();
}

void ConditionManager::addCondition(Condition* condition_)
{
	conditionlist.push_back(condition_);
}

Condition* ConditionManager::getConditon(std::string nameCondition_)
{
	for (auto condition : conditionlist) 
	{
		if (condition->getName() == nameCondition_)
		{
			return condition;
		}
	}

	return nullptr;
}

ConditionManager::ConditionManager() : distanceinfo(4)
{

}

ConditionManager::~ConditionManager()
{
}
