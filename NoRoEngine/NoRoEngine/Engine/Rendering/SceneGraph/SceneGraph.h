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
#include "../../Animation/animatedModel/AnimatedModel.h"

class SceneGraph
{
public:
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator = (const SceneGraph&) = delete;
	SceneGraph& operator = (SceneGraph&&) = delete;

	static SceneGraph* GetInstance();
	void AddModel(Model* model_);
	void AddModel(Model* model_, string name_);
	Model* getModel(string  modelName_);
	AnimatedModel* getAniModel(string  modelName_);
	void AddGameObject(GameObject* go_, string tag_ = "");
	void AddGuiObject(GuiObject* go_, string tag_ = "");
	bool isExist(string tag);
	GameObject* getGameObject(string tag_);
	GuiObject* getGuiObject(string tag_);

	void Update(const float deltatime_);

	void UpdateClick(const float deltatime_, Graph<Node> grid, SDL_Event e_);

	void Render(Camera* camera_);
	void Render(Camera* camera_, unsigned int  FBO_, unsigned int depthMap);
	void Draw(Camera* camera_);
	void OnDestroy();
	void setCharacter(GameObject* character_);
	void setTarget(GameObject* target_);
	void setTargetList(GameObject* target_);
	void setupSeek();
	void setupArrive();
	void StopMoving();
	void DelayedRender(const float deltatime_);
	void Remove(string tag);
	void RemoveObj(string tag);
	void RemoveGUI(string tag);
	//set up all needed information for avoidance
	void setupCollisionAvoidance();
	void PlayerMoving(string tagName, vec3 pos_);

	void RPGPlayerMoving(const float deltatime_);
	void RPGPlayerMove(const float deltatime_, string tag_, bool inputs[4]);
	void RenderQuad();
	void setGrid(Graph<Node> gird);

private:

	void Move(GameObject* player_, vec3 _inputDirection,vec3 angularVelocity, const float time_);
	Seek* seek;
	Arrive* arrivetest;
	//create point to avoidance clas
	CollisionAvoidance* avoidance;

	GameObject* target;
	GameObject* character;

	//list of targert
	vector<GameObject*> targetList;
	vector<GameObject*> delayUpdate;

	SceneGraph();
	~SceneGraph();
	string testTag;
	static unique_ptr<SceneGraph> sceneGraphInstance;
	friend default_delete<SceneGraph>;
	static map<GLuint, vector<Model*>> sceneModels;
	static map<string, GameObject*> sceneGameObjects;
	std::map<string, GameObject*>::iterator it;
	std::map<string, GuiObject*>::iterator guiit;
	static map<string, GuiObject*> sceneGuiObjects;

	int currentPathIndex;
	std::vector<vec3> movingPath;
	AStarPathFinding* path;
};
#endif // !SCENEGRAPH_H


