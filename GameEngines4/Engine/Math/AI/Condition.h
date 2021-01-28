#ifndef CONDITION_H
#define CONDITION_H

#include <memory>
#include <string>

using namespace std;
class Condition
{
public:
	
	Condition();
	virtual ~Condition();
	virtual bool Test() = 0;
	virtual std::string getName() = 0;
	virtual bool setResult(float valueA, float valueB) = 0;

	//bool collide();
	//bool noCollide();

	//void setCheck(bool check_) { check = check_; };
	//void setOutside(bool check_) { outside = check_; };
	//void setInside(bool check_) { inside = check_; };

private:

	//bool check;
	//bool outside, inside;
	//int minValue;

};
#endif // !CONDITION_H



