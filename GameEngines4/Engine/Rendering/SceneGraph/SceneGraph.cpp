#include "SceneGraph.h"

unique_ptr<SceneGraph> SceneGraph::sceneGraphInstance = nullptr;
map<GLuint, vector<Model*>> SceneGraph::sceneModels = map<GLuint, vector<Model*>>();
map<string, GameObject*> SceneGraph::sceneGameObjects = map < string, GameObject*>();
map<string, GuiObject*> SceneGraph::sceneGuiObjects = map < string, GuiObject*>();



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

void SceneGraph::AddGuiObject(GuiObject* go_, string tag_)
{
	testTag = tag_;
	if (tag_ == "")
	{
		string newTag = "GuiObject" + to_string(sceneGameObjects.size() + 1);
		go_->SetTag(newTag);
		sceneGuiObjects[newTag] = go_;
	}
	else if (sceneGameObjects.find(tag_) == sceneGameObjects.end())
	{
		go_->SetTag(tag_);
		sceneGuiObjects[tag_] = go_;
	}
	else
	{
		Debug::Error("Try to add GuiObject with tag: '" + tag_ + "' that already exits", "SceneGraph", __LINE__);
		string newTag = "GuiObject" + to_string(sceneGuiObjects.size() + 1);
		go_->SetTag(newTag);
		sceneGuiObjects[newTag] = go_;
	}

}

GameObject* SceneGraph::getGameObject(string tag_)
{
	if (sceneGameObjects.find(tag_) != sceneGameObjects.end())
	{
		return sceneGameObjects[tag_];
	}
	return nullptr;
}

GuiObject* SceneGraph::getGuiObject(string tag_)
{
	if (sceneGuiObjects.find(tag_) != sceneGuiObjects.end())
	{
		return sceneGuiObjects[tag_];
	}
	return nullptr;
}

void SceneGraph::Update(const float deltatime_)
{
	for (auto go : sceneGameObjects)
	{
		if (go.first == "apple")
		{
<<<<<<< Updated upstream
			go.second->Update(test->getSteering(), deltatime_);
=======
			//go.second->Update(arrivetest->getSteering(), deltatime_);
			go.second->Update(test->getSteering(), deltatime_);
		}

		if (go.first == "DICE")
		{
>>>>>>> Stashed changes
			go.second->Update(deltatime_);
			//go.second->Update(arrivetest->getSteering(), deltatime_);
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

void SceneGraph::Draw(Camera* camera_)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint guiProgram = ShaderHandler::getInstance()->GetShader("spriteShader");
	glUseProgram(guiProgram);

	for (auto entry : sceneGuiObjects)
	{
		entry.second->Draw(camera_);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
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
	
	if (sceneGuiObjects.size() > 0)
	{
		for (auto go : sceneGuiObjects)
		{
			delete go.second;
			go.second = nullptr;
		}
		sceneGuiObjects.clear();
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
<<<<<<< Updated upstream
=======

}

void SceneGraph::setupArrive()
{
	arrivetest = new Arrive(character, target);
	arrivetest->setmaxAcceleration(7.0f);
	arrivetest->setmaxSpeed(2.0f);
	arrivetest->setslowRadius(0.5f);
	arrivetest->settargetRadius(2.0f);

>>>>>>> Stashed changes
}
