#include "MovebackActions.h"
#include "ConditionManager.h"


MovebackActions::MovebackActions(GameObject* gameobject_)
{
	gameObject = gameobject_;
	avoidance = new CollisionAvoidance(gameObject, SceneGraph::GetInstance()->getGameObject("DICE"), targetList);
	avoidance->setmaxAcceleration(1.0f);
	avoidance->setRadius(3.0f);
}

MovebackActions::~MovebackActions()
{

}

void MovebackActions::DoSomething(const float deltaTime_)
{
	if (SceneGraph::GetInstance()->getGameObject(gameObject->GetTag()) != nullptr)
	{
		if (gameObject->GetPosition() == vec3(8, 0, -5))
		{
			ConditionManager::GetInstance()->setDistance(4);
		}
		else
		{
			//ConditionManager::GetInstance()->getConditon("floatIdle")->setResult(6, 7);
			//ConditionManager::GetInstance()->getConditon("floatAttack")->setResult(6, 7);
			//gameObject->MoveTest(deltaTime_);
		}
		
	}
}
