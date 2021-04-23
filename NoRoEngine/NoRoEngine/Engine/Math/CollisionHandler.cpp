#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"

unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
vector<GameObject*> CollisionHandler::prevCollisions = vector<GameObject*>();
vector<GuiObject*> CollisionHandler::prevGuiCollisions = vector<GuiObject*>();
OctSpatialPartition * CollisionHandler::scenePartition  = nullptr;


CollisionHandler::CollisionHandler()
{
	//colliders.reserve(10);
	prevCollisions.reserve(10);
}


CollisionHandler::~CollisionHandler()
{
	OnDestroy();
}

CollisionHandler * CollisionHandler::GetInstance()
{
	if (collisionInstance.get() == nullptr)
	{
		
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::OnDestroy()
{
	//for (auto entry : colliders)
	//{
	//	entry = nullptr;
	//}
	//colliders.clear();

	for (auto entry : prevCollisions)
	{
		entry = nullptr;
	}
	prevCollisions.clear();
	
	delete scenePartition;
	scenePartition = nullptr;
}

void CollisionHandler::OnCreate(float worldSize)
{
	//colliders.clear();
	prevCollisions.clear();
	scenePartition = new OctSpatialPartition(worldSize);
}

void CollisionHandler::AddGameObject(GameObject * go_)
{
	//colliders.push_back(go_);
	scenePartition->AddObject(go_);
}

void CollisionHandler::MouseUpdate(vec2 mousePosition_, int buttonType_)
{
	Ray mouseRay = CollisionDetection::ScreenPosToWorldRay(mousePosition_, CoreEngine::GetInstance()->GetWindowSize(), CoreEngine::GetInstance()->GetCamera());
	if (scenePartition != nullptr)
	{
		GameObject * hitResult = scenePartition->GetCollision(mouseRay);

		if (hitResult)
		{
			hitResult->SetHit(true, buttonType_);
		}

		for (auto c : prevCollisions)
		{
			if (c != hitResult && c != nullptr)
			{
				c->SetHit(false, buttonType_);
				c = nullptr;
			}
		}

		prevCollisions.clear();

		if (hitResult)
		{
			prevCollisions.push_back(hitResult);
		}
	}
	
}

/*GameObject * hitResult = nullptr;
float shortestDist = FLT_MAX;

for (auto go : colliders)
{
	if (mouseRay.isColliding(&go->GetBoundingBox()))
	{
		if (mouseRay.intersectionDist < shortestDist)
		{
			hitResult = go;
			shortestDist = mouseRay.intersectionDist;
		}
	}
}*/