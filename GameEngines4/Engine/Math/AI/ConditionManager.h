#ifndef CONDITIONMANAGER_H
#define CONDITIONMANAGER_H

#include <memory>
#include <vector>

#include "Condition.h"

using namespace std;
class ConditionManager
{
public:
	ConditionManager(const ConditionManager&) = delete;
	ConditionManager(ConditionManager&&) = delete;
	ConditionManager& operator = (const ConditionManager&) = delete;
	ConditionManager& operator = (ConditionManager&&) = delete;

	static ConditionManager* GetInstance();
	void addCondition(Condition * condition_);
	Condition* getConditon(std::string nameCondition_);
	void setDistance(float distance_) { distanceinfo = distance_; };
	float getDistance() { return distanceinfo; };
private:
	ConditionManager();
	~ConditionManager();
	static unique_ptr<ConditionManager> conditionInstance;
	friend default_delete<ConditionManager>;
	float distanceinfo;
	std::vector<Condition*> conditionlist;
};

#endif // !CONDITIONMANAGER_H



