#ifndef ACTIONS_H
#define ACTIONS_H

#include <iostream>

using namespace std;

enum ACTION
{
    HIT,
    IDLE,
    RUN
};

class Actions
{
public:
	Actions();
	~Actions();
    Actions(int i);

    void DoSomething();
    Actions operator+=(Actions &b)
    {
        this->DoSomething();
        b.DoSomething();
        return *this;
    }

private:

};
#endif // !ACTIONS_H



