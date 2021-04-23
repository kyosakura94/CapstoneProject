#include "NetworkScene.h"

NetworkScene::NetworkScene()
{
}

NetworkScene::~NetworkScene()
{
}

bool NetworkScene::OnCreate()
{
	CoreEngine::GetInstance()->SetCamera(new Camera);
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(5.0f, 5.0f, 0.0f));
	CoreEngine::GetInstance()->GetCamera()->SetRotation(-90, -45);
	CoreEngine::GetInstance()->GetCamera()->Addlightsources(new LightSource(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.8f));
	CollisionHandler::GetInstance()->OnCreate(100.f);

	AnimatedModel* BlueDiceModel = new AnimatedModel(
		"./Resources/Models/Dice.obj",
		"./Resources/Materials/Dice.mtl",
		ShaderHandler::getInstance()->GetShader("basicShader"),
		CoreEngine::GetInstance()->getRendererType());

	AnimatedModel* RedDiceModel = new AnimatedModel(
		"./Resources/Models/Dice1.obj",
		"./Resources/Materials/Dice1.mtl",
		ShaderHandler::getInstance()->GetShader("basicShader"),
		CoreEngine::GetInstance()->getRendererType());


	SceneGraph::GetInstance()->AddModel(BlueDiceModel, "blueDice");
	SceneGraph::GetInstance()->AddModel(RedDiceModel, "redDice");

	//GameObject* BlueDiceObj = new GameObject(BlueDiceModel, vec3(2.0f, 0.0f, -5.0f));

	//SceneGraph::GetInstance()->AddGameObject(BlueDiceObj, "DICE");
	for (size_t i = 0; i < 4; i++)
	{
		input[i] = false;
	}

	return true;
}

void NetworkScene::Update(const float deltaTime_)
{
	if (KeyEventListener::GetKeyState("W"))
	{
		input[0] = true;
	}
	if (KeyEventListener::GetKeyState("A"))
	{
		input[1] = true;
	}	
	if (KeyEventListener::GetKeyState("S"))
	{
		input[2] = true;
	}	
	if (KeyEventListener::GetKeyState("D"))
	{
		input[3] = true;
	}


	//SceneGraph::GetInstance()->RPGPlayerMoving(deltaTime_);
	SceneGraph::GetInstance()->RPGPlayerMove(deltaTime_, "Player1", input);


	if (Client::getInstance()->IsConnected())
	{
		Packet* packet = Client::getInstance()->ReceiveAPacket(deltaTime_);

		if (packet != nullptr)
		{
			switch (packet->GetType())
			{
			case PACKET_UPDATE_POSITION:

				const UpdatePosition* data;
				data = (UpdatePosition*)packet;

				SceneGraph::GetInstance()->PlayerMoving((char*)data->tagName, data->position);
				break;

			case TEST_PACKET_CREATEPLAYER:

				const TestPacket* tmp;
				tmp = (TestPacket*)packet;
				CreatePlayer("");
				//CreatePlayer(tmp->position, (char*)tmp->modelName, (char*)tmp->tagName);

				//createCheck = false;
				break;

			case PACKET_JSON:
				const JsonPacket* jsonPacket;
				jsonPacket = (JsonPacket*)packet;

				CreatePlayer((char*)jsonPacket->jsonString);

				ConnectionKeepAlivePacket* packet = (ConnectionKeepAlivePacket*)Client::getInstance()->getPacketFactory()->CreatePacket(PACKET_CONNECTION_KEEP_ALIVE);
				Client::getInstance()->SendPackets(packet);

				break;
			}
		}
	}


	for (size_t i = 0; i < 4; i++)
	{
		input[i] = false;
	}

}

void NetworkScene::Render()
{
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}

void NetworkScene::Draw()
{
}

void NetworkScene::CreatePlayer(vec3 pos, string modelName, string tag)
{
	if (SceneGraph::GetInstance()->getGameObject(tag) == nullptr)
	{
		//GameObject* BlueDiceObj = new GameObject(SceneGraph::GetInstance()->getModel(modelName), pos);
		//SceneGraph::GetInstance()->AddGameObject(BlueDiceObj, tag);
	}
}

void NetworkScene::CreatePlayer(string j)
{
	std::cout << "Json Received: " << j << std::endl;

	json j3 = json::parse(j);

	for (auto &info : j3["playerInfo"])
	{
		std::string tag = info["tag"].get<std::string>();
		std::string modelName = info["modelName"].get<std::string>();

		std::vector<float> position;
		for (auto& elem : info["position"]) position.push_back(elem);

		if (SceneGraph::GetInstance()->isExist(tag) != true)
		{
			//GameObject* BlueDiceObj = new GameObject(SceneGraph::GetInstance()->getModel(modelName), vec3(position[0], position[1], position[2]));
			//SceneGraph::GetInstance()->AddGameObject(BlueDiceObj, tag);
		}
	}
}

