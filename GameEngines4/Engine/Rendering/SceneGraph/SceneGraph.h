#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include<memory.h>
#include <glm/gtx/string_cast.hpp>
#include <map>
#include <vector>
#include "../3D/GameObject.h"
#include "../2D/GuiObject.h"
#include "../../Core/CoreEngine.h"
#include "../../Math/CollisionHandler.h"
#include "../../Math/AI/Seek.h"
#include "../../Math/AI/Arrive.h"
#include "../../Math/AI/CollisionAvoidance.h"
#include "../../Math/AI/Graph.h"
#include "../../Math/AI/node.h"
#include "../../Math/AI/AStarPathFinding.h"

class SceneGraph
{
public:
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator = (const SceneGraph&) = delete;
	SceneGraph& operator = (SceneGraph&&) = delete;

	static SceneGraph *GetInstance();
	void AddModel(Model * model_);
	void AddGameObject(GameObject * go_, string tag_ ="");
	void AddGuiObject(GuiObject * go_, string tag_ ="");

	GameObject * getGameObject(string tag_);
	GuiObject * getGuiObject(string tag_);

	void Update(const float deltatime_);

	void UpdateClick(const float deltatime_, Graph<Node> grid, SDL_Event e_);

	void Render(Camera * camera_);
	void Draw(Camera * camera_);
	void OnDestroy();
	void setCharacter(GameObject *character_);
	void setTarget(GameObject *target_);
	void setTargetList(GameObject *target_);
	void setupSeek();
	void setupArrive();
	void StopMoving();
	//set up all needed information for avoidance
	void setupCollisionAvoidance();
private:
	Seek* seek;
	Arrive* arrivetest;
	//create point to avoidance clas
	CollisionAvoidance* avoidance;

	GameObject* target;
	GameObject* character; 

	//list of targert
	vector<GameObject*> targetList;

	SceneGraph();
	~SceneGraph();
	string testTag;
	static unique_ptr<SceneGraph> sceneGraphInstance;
	friend default_delete<SceneGraph>;
	static map<GLuint, vector<Model*>> sceneModels;
	static map<string, GameObject*> sceneGameObjects;
	static map<string, GuiObject*> sceneGuiObjects;
	int currentPathIndex;
	std::vector<vec3> movingPath;

};
#endif // !SCENEGRAPH_H


