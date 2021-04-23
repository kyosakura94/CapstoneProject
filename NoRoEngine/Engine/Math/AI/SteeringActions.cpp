#include "SteeringActions.h"

SteeringActions::SteeringActions(GameObject* gameobject_)
{
	gameObject = gameobject_;
	avoidance = new CollisionAvoidance(gameObject, SceneGraph::GetInstance()->getGameObject("DICE"), targetList);
	avoidance->setmaxAcceleration(1.0f);
	avoidance->setRadius(3.0f);
}

SteeringActions::~SteeringActions()
{

}

void SteeringActions::DoSomething(const float deltaTime_)
{
	cout << "Do something " << endl;

	if (SceneGraph::GetInstance()->getGameObject(gameObject->GetTag()) != nullptr)
	{
		gameObject->Move(avoidance->getSteering(), deltaTime_);
	}
}
