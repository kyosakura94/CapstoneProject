#ifndef CONDITION_H
#define CONDITION_H

#include <memory>

using namespace std;
class Condition
{
public:
	Condition(const Condition&) = delete;
	Condition(Condition&&) = delete;
	Condition& operator = (const Condition&) = delete;
	Condition& operator = (Condition&&) = delete;

	static Condition* GetInstance();

	bool Test();
	bool collide();
	bool noCollide();

	void setCheck(bool check_) { check = check_; };
	void setOutside(bool check_) { outside = check_; };
	void setInside(bool check_) { inside = check_; };
private:

	Condition();
	~Condition();
	static unique_ptr<Condition> conditionInstance;
	friend default_delete<Condition>;
	bool check;
	bool outside, inside;
	int minValue;

};
#endif // !CONDITION_H



