#include "DoNothingActions.h"


DoNothingActions::DoNothingActions(GameObject* gameobject_)
{
	gameObject = gameobject_;
	avoidance = new CollisionAvoidance(gameObject, SceneGraph::GetInstance()->getGameObject("DICE"), targetList);
	avoidance->setmaxAcceleration(1.0f);
	avoidance->setRadius(3.0f);
}

DoNothingActions::~DoNothingActions()
{

}

void DoNothingActions::DoSomething(const float deltaTime_)
{
	cout << "Do nothing" << endl;
}
