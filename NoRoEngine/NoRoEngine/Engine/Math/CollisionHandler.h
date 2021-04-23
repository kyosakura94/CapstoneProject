#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Ray.h"
#include "../Rendering/3D/GameObject.h"
#include "../Rendering/2D/GuiObject.h"
#include "../Core/OctSpatialPartition.h"

using namespace std;
using namespace glm;
class CollisionHandler 
{
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator = (const CollisionHandler&) = delete;
	CollisionHandler& operator = (CollisionHandler&&) = delete;

	static CollisionHandler * GetInstance();
	void OnDestroy();
	void OnCreate(float worldSize);
	void AddGameObject(GameObject * go_);
	void MouseUpdate(vec2 mousePosition_, int buttonType_);

private:

	CollisionHandler();
	~CollisionHandler();

	static unique_ptr<CollisionHandler> collisionInstance;
	friend default_delete<CollisionHandler>;

	//static vector<GameObject*> colliders;
	static vector<GameObject*> prevCollisions;
	static vector<GuiObject*> prevGuiCollisions;
	static OctSpatialPartition * scenePartition;

};
#endif
