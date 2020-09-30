#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include<memory.h>
#include <map>
#include <vector>
#include "../3D/GameObject.h"
#include "../../Math/CollisionHandler.h"


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
	GameObject * getGameObject(string tag_);
	void Update(const float deltatime_);
	void Render(Camera * camera_);
	void OnDestroy();

private:

	SceneGraph();
	~SceneGraph();
	string testTag;
	static unique_ptr<SceneGraph> sceneGraphInstance;
	friend default_delete<SceneGraph>;
	static map<GLuint, vector<Model*>> sceneModels;
	static map<string, GameObject*> sceneGameObjects;

};
#endif // !SCENEGRAPH_H


