#include "SceneGraph.h"
#include "../../../Game/Scene/GameScene.h"

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
		string newTag = "GuiObject" + to_string(sceneGuiObjects.size() + 1);
		go_->SetTag(newTag);
		sceneGuiObjects[newTag] = go_;
	}
	else if (sceneGuiObjects.find(tag_) == sceneGuiObjects.end())
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

bool SceneGraph::isExist(string tag)
{
	for (auto obj : sceneGameObjects)
	{
		if (obj.first == tag)
		{
			return true;
		}
		else
		{
			return false; 
		}
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
	//for (auto go : sceneModels)
	//{
	//	for (auto s : go.second)
	//	{
	//		if (s->getJointCount() != 0)
	//		{
	//			s->update(deltatime_);
	//		}
	//	}
	//}


	for (auto it = sceneGameObjects.cbegin(), next_it = it; it != sceneGameObjects.cend(); it = next_it)
	{
		++ next_it;

		if (it->second->getLifetime() < 0)
		{
			Remove(it->first);
		}
		else
		{
			it->second->Update(deltatime_);
		}
	}

	//for (auto go : sceneGameObjects)
	//{
	//	//if (go.second->GetHit() == true)
	//	//{
	//	//	//cout << glm::to_string(go.second->GetPosition()) << std::endl << endl;
	//	//}

	//	//if (go.first == "apple")
	//	//{
	//	//	
	//	//	//go.second->Update(seek->getSteering(), deltatime_);
	//	//go.second->Update(avoidance->getSteering(), deltatime_);
	//	//}
	//	//else
	//	//{

	//	//	//vec3 targetPosition(5, 0, -10);

	//	//	//vec3 diff = targetPosition - go.second->GetPosition();
	//	//	//float distance = sqrtf(dot(diff, diff));

	//	//	//if (distance > 1.0f)
	//	//	//{
	//	//	//	vec3 moveDir = normalize(diff);
	//	//	//	vec3 pos = go.second->GetPosition() + moveDir * 2.0f * deltatime_;
	//	//	//	go.second->SetPosition(pos);
	//	//	//}
	//	//	//go.second->Update(deltatime_);
	//	//	//if (go.first == "DICE")
	//	//	//{
	//	//	//	go.second->Update(deltatime_);
	//	//	//	//go.second->Update(arrivetest->getSteering(), deltatime_);
	//	//	//}
	//	//	//else
	//	//	//{
	//	//	//	
	//	//	//}
	//	//}


	//	//if (go.first == "apple")
	//	//{
	//	//	cout << "Check" << endl;
	//	//}
	//	if (go.second != nullptr)
	//	{
	//		go.second->Update(deltatime_);

	//	}
	//	


	//	//if (go.first == "animatedObj" )//|| go.first == "redPandaObj")
	//	//{
	//	//	
	//	//}
	//}


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
void SceneGraph::Remove(string tag)
{
	int modelinstane = 0;
	string glpro;

	if (sceneGameObjects.size() > 0)
	{
		modelinstane = sceneGameObjects[tag]->getModelIntances();
		glpro = sceneGameObjects[tag]->getModel()->getModelName();
		sceneGameObjects[tag]->Destroy();

		it = sceneGameObjects.find(tag);
		sceneGameObjects.erase(it);

	}

	if (sceneModels.size() > 0)
	{
		for (auto entry : sceneModels)
		{
			for (vector<Model*>::iterator it = entry.second.begin(); it != entry.second.end();)
			{
				string name = (*it)->getModelName();

				if (name == glpro)
				{
					(*it)->RemoveInstance(modelinstane);
					break;
				}
				else
				{
					++it;
				}
			}
		}
	}
}
void SceneGraph::RemoveObj(string tag)
{
	int modelinstane = 0;
	string glpro;

	if (sceneGameObjects.size() > 0)
	{
		modelinstane = sceneGameObjects[tag]->getModelIntances();
		glpro = sceneGameObjects[tag]->getModel()->getModelName();
		sceneGameObjects[tag]->Destroy();

		it = sceneGameObjects.find(tag);
		sceneGameObjects.erase(it);
	}
}
void SceneGraph::RemoveGUI(string tag)
{
	if (sceneGuiObjects.size() > 0)
	{
		if (sceneGuiObjects[tag])
		{
			sceneGuiObjects[tag]->Destroy();

			guiit = sceneGuiObjects.find(tag);
			sceneGuiObjects.erase(guiit);
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

void SceneGraph::Render(Camera* camera_, unsigned int FBO_, unsigned int depthMap)
{
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------

	// render scene from light's point of view
	GLuint guiProgram = ShaderHandler::getInstance()->GetShader("simpleDepthShader");
	glUseProgram(guiProgram);

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 0.1f, far_plane = 100.0f;
	lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
	lightView = glm::lookAt(camera_->GetLightSources()[0]->GetLightPosition(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	glUniformMatrix4fv(glGetUniformLocation(guiProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix[0]));

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_);

	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);

	////RENDER SCENE
	for (auto entry : sceneModels)
	{
		for (auto m : entry.second)
		{
			m->ShadowRender(camera_, guiProgram);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glViewport(0, 0, 1080, 720);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------


	glViewport(0, 0, 1080, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto entry : sceneModels)
	{
		glUseProgram(entry.first);

		for (auto m : entry.second)
		{
			//m->Render(camera_, testTag);
			m->Render(camera_, testTag, entry.first , depthMap, lightSpaceMatrix);
		}
	}

	//// render Depth map to quad for visual debugging
	//// ---------------------------------------------

	GLuint debugguiProgram = ShaderHandler::getInstance()->GetShader("debugDepthQuad");
	glUseProgram(debugguiProgram);

	glUniform1f(glGetUniformLocation(debugguiProgram, "near_plane"), near_plane);
	glUniform1f(glGetUniformLocation(debugguiProgram, "far_plane"), far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//RenderQuad();
}

void SceneGraph::Draw(Camera* camera_)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint guiProgram = ShaderHandler::getInstance()->GetShader("spriteShader");
	glUseProgram(guiProgram);

	if (sceneGuiObjects.size() > 0)
	{
		for (auto entry : sceneGuiObjects)
		{
			entry.second->Draw(camera_);
		}
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
	float speed = 10.0f;
	GameObject* player_ = getGameObject(tag_);

	//if (inputs[0])
	//{
	//	quat df;
	//	vec3 EulerAngles(0, -40, 0);
	//	df = quat(EulerAngles);
	//	player_->SetQuaternion(df);
	//}


	////player_->SetAccel(vec3(0,0,0));
	//player_->SetVelocity(vec3(0,0,0));

	//Physics::SimpleNewtonMotion(*player_, deltatime_);

	vec3 _inputDirection = vec3(0);
	vec3 angelVelocity = vec3(0);

	if (inputs[0])
	{
		angelVelocity.y = glm::radians(180.0f);
		_inputDirection.z -= speed * deltatime_;
	}

	if (inputs[1])
	{
		angelVelocity.y = glm::radians(-90.0f);
		_inputDirection.x -= speed * deltatime_;
	}

	if (inputs[2])
	{
		angelVelocity.y = glm::radians(0.0f);
		_inputDirection.z += speed * deltatime_;	
	}
	if (inputs[3])
	{
		angelVelocity.y = glm::radians(90.0f);
		_inputDirection.x += speed * deltatime_;
	}

	if (player_ != nullptr && _inputDirection != vec3(0))
	{
		Move(player_, _inputDirection, angelVelocity, deltatime_);
	}
}

unsigned int quadVAO = 0;
unsigned int quadVBO;

void SceneGraph::RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void SceneGraph::Move(GameObject * player_, vec3 _inputDirection, vec3 angularVelocity, const float time_)
{
	float elapsedTime = (float)SDL_GetTicks() / 1000;
	vec3 forwardVector = Physics::ComputeForwardVector(player_->getQuaternion());
	cout << "X:" << forwardVector.x << " Y:" << forwardVector.y << " Z:" << forwardVector.z << endl;


	vec3 test(1,0,0);
	quat q(vec3(0,glm::radians(90.0f),0));

	quat q1 = normalize(q);

	quat qv(0,1,0,0);

	quat result = q * qv * q1;
	vec3 s = Physics::ComputeForwardVector(result);

	//player_->SetVelocity(vec3(0,0,0) * elapsedTime);
	//player_->SetAngVel(angularVelocity);

	quat df(angularVelocity);

	player_->SetQuaternion(df);


	vec3 newforwardVector = Physics::ComputeForwardVector(player_->getQuaternion());
	normalize(newforwardVector);
	cout <<"New forward "<< "X:" << newforwardVector.x << " Y:" << newforwardVector.y << " Z:" << newforwardVector.z << endl;


	//Physics::SimpleNewtonMotion(*player_, time_);

	vec3 pos = player_->GetPosition() + _inputDirection;
	player_->SetPosition(pos);



	//vec3 forward = Physics::ComputeForwardVector(player_->getQuaternion());

	//float elapsedTime = (float)SDL_GetTicks() / 1000;

	//cout << "RUN" << endl;

	//cout << "X:" << forward.x << " Y:" << forward.y << " Z:" << forward.z << endl;

	//quat df;
	//vec3 EulerAngles(0, glm::radians(-90.0f) * elapsedTime, 0);
	//vec3 direction(glm::radians(-90.0f), 0, 0);

	//df = quat(EulerAngles);

	//vec3 desiredUp(0, 1.0f, 0);


	//quat rot1 = Physics::RotationBetweenVectors(vec3(0.0f, 0.0f, 1.0f), direction);
	//vec3 right = cross(direction, desiredUp);
	//desiredUp = cross(right, direction);

	//vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);

	//quat rot2 = Physics::RotationBetweenVectors(newUp, desiredUp);
	//quat targetOrientation = rot2 * rot1;
	//quat CurrentOrientation = Physics::RotateTowards(player_->getQuaternion(), df, 3.14f * elapsedTime);
	//player_->SetQuaternion(CurrentOrientation);
	//cout << "X:" << forward.x << " Y:" << forward.y << " Z:" << forward.z << endl;

	//For update to server
	//UpdatePosition* packet = (UpdatePosition*)Client::getInstance()->getPacketFactory()->CreatePacket(PACKET_UPDATE_POSITION);
	//packet->setUpData(player_->GetTag(), pos);
	//Client::getInstance()->SendPackets(packet);
}

void SceneGraph::setGrid(Graph<Node> gird)
{
	path = new AStarPathFinding(gird);
}
