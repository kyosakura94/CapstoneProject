#ifndef FLOATCONDITION_H
#define FLOATCONDITION_H

#include "Condition.h"
class FloatCondition : public Condition
{
public:

	FloatCondition(std::string name_);

	virtual ~FloatCondition();
	virtual bool Test();
	virtual std::string getName();

	virtual bool setResult(float valueA, float valueB);

private:
	bool result;
	string conditionName;
};
#endif // !FLOATCONDITION_H



