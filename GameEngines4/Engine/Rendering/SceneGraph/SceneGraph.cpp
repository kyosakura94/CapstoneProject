#include "SceneGraph.h"

unique_ptr<SceneGraph> SceneGraph::sceneGraphInstance = nullptr;
map<GLuint, vector<Model*>> SceneGraph::sceneModels = map<GLuint, vector<Model*>>();
map<string, GameObject*> SceneGraph::sceneGameObjects = map < string, GameObject*>();



SceneGraph::SceneGraph()
{
}


SceneGraph::~SceneGraph()
{
	OnDestroy();
}

SceneGraph * SceneGraph::GetInstance()
{
	if (sceneGraphInstance.get() == nullptr)
	{
		sceneGraphInstance.reset(new SceneGraph);
	}

	return sceneGraphInstance.get();
}

void SceneGraph::AddModel(Model * model_)
{
	if (sceneModels.find(model_->getShaderProgram()) == sceneModels.end())
	{
		vector<Model*> tmp = vector<Model*>();
		tmp.reserve(10);
		tmp.push_back(model_);
		sceneModels.insert(pair<GLuint, vector<Model*>>(model_->getShaderProgram(), tmp));
	}
	else
	{
		sceneModels[model_->getShaderProgram()].push_back(model_);
	}
}

void SceneGraph::AddGameObject(GameObject * go_, string tag_)
{
	testTag = tag_;
	if (tag_ == "")
	{
		string newTag = "GameObject" + to_string(sceneGameObjects.size() + 1);
		go_->SetTag(newTag);
		sceneGameObjects[newTag] = go_;
	}
	else if(sceneGameObjects.find(tag_) == sceneGameObjects.end())
	{
		go_->SetTag(tag_);
		sceneGameObjects[tag_] = go_;
	}
	else 
	{
		Debug::Error("Try to add gameObject with tag: '" + tag_ + "' that already exits", "SceneGraph", __LINE__);
		string newTag = "GameObject" + to_string(sceneGameObjects.size() + 1);
		go_->SetTag(newTag);
		sceneGameObjects[newTag] = go_;		
	}

	CollisionHandler::GetInstance()->AddGameObject(go_);
}

GameObject* SceneGraph::getGameObject(string tag_)
{
	if (sceneGameObjects.find(tag_) != sceneGameObjects.end())
	{
		return sceneGameObjects[tag_];
	}
	return nullptr;
}

void SceneGraph::Update(const float deltatime_)
{
	for (auto go : sceneGameObjects)
	{
		if (go.first == "apple")
		{
			go.second->Update(test->getSteering(), deltatime_);
			go.second->Update(deltatime_);
		}
	}
}

void SceneGraph::Render(Camera * camera_)
{
	for (auto entry : sceneModels)
	{
		glUseProgram(entry.first);
		for (auto m : entry.second)
		{
			m->Render(camera_, testTag);
		}
	}
}

void SceneGraph::OnDestroy()
{
	if (sceneGameObjects.size() > 0)
	{
		for (auto go : sceneGameObjects)
		{
			delete go.second;
			go.second = nullptr;
		}
		sceneGameObjects.clear();
	}
	if (sceneModels.size() > 0)
	{
		for (auto entry : sceneModels)
		{
			if (entry.second.size() > 0)
			{
				for (auto m : entry.second)
				{
					delete m;
					m = nullptr;
				}
				entry.second.clear();
			}
		}
		sceneModels.clear();
	}
}

void SceneGraph::setCharacter(GameObject* character_)
{
	character = character_;
}

void SceneGraph::setTarget(GameObject* target_)
{
	target = target_;
}

void SceneGraph::setupSeek()
{
	test = new Seek(character, target);
	test->setmaxAcceleration(5.0f);
}
