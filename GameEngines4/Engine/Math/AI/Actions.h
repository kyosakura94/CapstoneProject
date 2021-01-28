#ifndef ACTIONS_H
#define ACTIONS_H

#include <iostream>
#include "../../Rendering/3D/GameObject.h"
#include "../../Rendering/SceneGraph/SceneGraph.h"


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
	virtual ~Actions();

    //Actions(int i);
    //Actions(GameObject *gameobject_);

    virtual void DoSomething(const float deltaTime_) = 0;

    //Actions operator+=(Actions &b)
    //{
    //    this->DoSomething();
    //    b.DoSomething();
    //    return *this;
    //}

private:
    //CollisionAvoidance* avoidance;
    //std::vector<GameObject*> targetList;
    //GameObject* gameObject;
};
#endif // !ACTIONS_H



