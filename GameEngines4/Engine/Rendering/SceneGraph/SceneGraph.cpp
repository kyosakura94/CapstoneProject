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

SceneGraph* SceneGraph::GetInstance()
{
	if (sceneGraphInstance.get() == nullptr)
	{
		sceneGraphInstance.reset(new SceneGraph);
	}

	return sceneGraphInstance.get();
}

void SceneGraph::AddModel(Model* model_)
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

void SceneGraph::AddModel(Model* model_, string name_)
{
	model_->setModelName(name_);

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

Model* SceneGraph::getModel(string modelName_)
{
	vector<Model*> tmp = sceneModels[ShaderHandler::getInstance()->GetShader("basicShader")];
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i]->getModelName() == modelName_)
		{
			return tmp[i];
		}
	}

	return nullptr;
}

void SceneGraph::AddGameObject(GameObject* go_, string tag_)
{
	testTag = tag_;
	if (tag_ == "")
	{
		string newTag = "GameObject" + to_string(sceneGameObjects.size() + 1);
		go_->SetTag(newTag);
		sceneGameObjects[newTag] = go_;
	}
	else if (sceneGameObjects.find(tag_) == sceneGameObjects.end())
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
		if (go.second->GetHit() == true)
		{
			//cout << glm::to_string(go.second->GetPosition()) << std::endl << endl;
		}

		if (go.first == "apple")
		{
			
			//go.second->Update(seek->getSteering(), deltatime_);
			go.second->Update(avoidance->getSteering(), deltatime_);
		}
		else
		{

			//vec3 targetPosition(5, 0, -10);

			//vec3 diff = targetPosition - go.second->GetPosition();
			//float distance = sqrtf(dot(diff, diff));

			//if (distance > 1.0f)
			//{
			//	vec3 moveDir = normalize(diff);
			//	vec3 pos = go.second->GetPosition() + moveDir * 2.0f * deltatime_;
			//	go.second->SetPosition(pos);
			//}
			//go.second->Update(deltatime_);
			//if (go.first == "DICE")
			//{
			//	go.second->Update(deltatime_);
			//	//go.second->Update(arrivetest->getSteering(), deltatime_);
			//}
			//else
			//{
			//	
			//}
		}

		//go.second->Update(deltatime_);
	}
}

void SceneGraph::UpdateClick(const float deltatime_, Graph<Node> grid, SDL_Event e_)
{
	for (auto go : sceneGameObjects)
	{
		if (go.second->GetDelayUpdate() != true)
		{
			if (go.second->GetHit() == true)
			{
				StopMoving();
				cout << glm::to_string(go.second->GetPosition()) << std::endl << endl;
				movingPath = path->FindPath(character, go.second);

				//movingPath = path->FindPath(0, 0, 4, 4);
				go.second->SetHit(false, 0);
				break;
			}
		}
	}


	if (movingPath.size() != 0)
	{
		vec3 targetPosition = movingPath[currentPathIndex];

		vec3 diff = targetPosition - character->GetPosition();
		float distance = sqrtf(dot(diff, diff));

		if (distance > 0.1f)
		{
			vec3 moveDir = normalize(diff);
			vec3 pos = character->GetPosition() + moveDir * 2.0f * deltatime_;
			character->SetPosition(pos);
		}
		else
		{
			currentPathIndex++;
			if (currentPathIndex >= movingPath.size())
			{
				StopMoving();
			}
		}
	}
}

void SceneGraph::StopMoving()
{
	currentPathIndex = 0;
	movingPath.clear();
}
void SceneGraph::DelayedRender(const float deltatime_)
{
	for (auto go : sceneGameObjects)
	{
		if (go.second->GetDelayUpdate() == true)
		{
			go.second->DelayRender(deltatime_);
		}
	}

}
void SceneGraph::Render(Camera* camera_)
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
void SceneGraph::setTargetList(GameObject* target_)
{
	targetList.push_back(target_);
}

void SceneGraph::setupSeek()
{
	seek = new Seek(character, target);
	seek->setmaxAcceleration(5.0f);

}

void SceneGraph::setupArrive()
{
	arrivetest = new Arrive(character, target);
	arrivetest->setmaxAcceleration(7.0f);
	arrivetest->setmaxSpeed(2.0f);
	arrivetest->setslowRadius(0.5f);
	arrivetest->settargetRadius(2.0f);

}

void SceneGraph::setupCollisionAvoidance()
{
	avoidance = new CollisionAvoidance(character, target, targetList);
	avoidance->setmaxAcceleration(1.0f);
	avoidance->setRadius(3.0f);
}

void SceneGraph::PlayerMoving(string tagName, vec3 pos_)
{
	for (auto go : sceneGameObjects)
	{
		if (go.first == tagName)
		{
			go.second->SetPosition(pos_);
		}
	}
}

void SceneGraph::RPGPlayerMoving(const float deltatime_)
{
	//for (auto go : sceneGameObjects)
	//{
	//	if (go.first == "Player1")
	//	{
	//		if (go.second->GetHit() == true)
	//		{
	//			vec3 targetPosition(5, 0, -10);
	//			vec3 diff = targetPosition - go.second->GetPosition();
	//			float distance = sqrtf(dot(diff, diff));

	//			if (distance > 1.0f)
	//			{
	//				vec3 moveDir = normalize(diff);
	//				vec3 pos = go.second->GetPosition() + moveDir * 2.0f * deltatime_;

	//				UpdatePosition* packet = (UpdatePosition*)Client::getInstance()->getPacketFactory()->CreatePacket(PACKET_UPDATE_POSITION);
	//				packet->setUpData(go.first, pos);

	//				Client::getInstance()->SendPackets(packet);
	//			}
	//		}
	//	}
	//}

	for (auto go : sceneGameObjects)
	{
		if (go.first == "Player1")
		{
			vec3 pos;
			vec3 targetPosition(5, 0, -10);
			vec3 diff = targetPosition - go.second->GetPosition();
			float distance = sqrtf(dot(diff, diff));

			if (distance > 0.1f)
			{
				vec3 moveDir = normalize(diff);
				pos = go.second->GetPosition() + moveDir * 2.0f * deltatime_;
				go.second->SetPosition(go.second->GetPosition() + moveDir * 2.0f * deltatime_);
			}

			UpdatePosition* packet = (UpdatePosition*)Client::getInstance()->getPacketFactory()->CreatePacket(PACKET_UPDATE_POSITION);
			packet->setUpData(go.first, go.second->GetPosition());
			Client::getInstance()->SendPackets(packet);
		}
	}
}

void SceneGraph::RPGPlayerMove(const float deltatime_, string tag_, bool inputs[4])
{
	GameObject * player_ = getGameObject(tag_);
	vec3 _inputDirection = vec3(0);

	if (inputs[0])
	{
		_inputDirection.z -= 0.1f;
	}
	if (inputs[1])
	{
		_inputDirection.x -= 0.1f;
	}
	if (inputs[2])
	{
		_inputDirection.z += 0.1f;
		
	}
	if (inputs[3])
	{
		_inputDirection.x += 0.1f;
	}

	if (player_ != nullptr)
	{
		Move(player_, _inputDirection);
	}
}

void SceneGraph::Move(GameObject * player_, vec3 _inputDirection)
{
	vec3 pos = player_->GetPosition() + _inputDirection;

	//player_->SetPosition(pos);

	UpdatePosition* packet = (UpdatePosition*)Client::getInstance()->getPacketFactory()->CreatePacket(PACKET_UPDATE_POSITION);
	packet->setUpData(player_->GetTag(), pos);

	Client::getInstance()->SendPackets(packet);
}

void SceneGraph::setGrid(Graph<Node> gird)
{
	path = new AStarPathFinding(gird);
}
