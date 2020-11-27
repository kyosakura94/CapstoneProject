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

	Model* BlueDiceModel = new Model(
		"./Resources/Models/Dice.obj",
		"./Resources/Materials/Dice.mtl",
		ShaderHandler::getInstance()->GetShader("basicShader"),
		CoreEngine::GetInstance()->getRendererType());

	Model* RedDiceModel = new Model(
		"./Resources/Models/Dice1.obj",
		"./Resources/Materials/Dice1.mtl",
		ShaderHandler::getInstance()->GetShader("basicShader"),
		CoreEngine::GetInstance()->getRendererType());


	SceneGraph::GetInstance()->AddModel(BlueDiceModel, "blueDice");
	SceneGraph::GetInstance()->AddModel(RedDiceModel, "redDice");

	//GameObject* BlueDiceObj = new GameObject(BlueDiceModel, vec3(2.0f, 0.0f, -5.0f));

	//SceneGraph::GetInstance()->AddGameObject(BlueDiceObj, "DICE");

	return true;
}

void NetworkScene::Update(const float deltaTime_)
{

	//SceneGraph::GetInstance()->RPGPlayerMoving(deltaTime_);
	//Server::getInstance()->ReceivePackets(deltaTime_);
	//Server::getInstance()->SendPackets(deltaTime_);

	
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
	GameObject* BlueDiceObj = new GameObject(SceneGraph::GetInstance()->getModel(modelName), pos);
	SceneGraph::GetInstance()->AddGameObject(BlueDiceObj, tag);
}

void NetworkScene::CreatePlayer(string j)
{
	std::cout << "Json Received: " << j << std::endl;

	auto j3 = json::parse(j);

	std::string tag = j3["tag"].get<std::string>();
	std::string modelName = j3["modelName"].get<std::string>();

	std::vector<float> position;
	for (auto& elem : j3["position"]) position.push_back(elem);


	GameObject* BlueDiceObj = new GameObject(SceneGraph::GetInstance()->getModel(modelName), vec3(position[0], position[1], position[2]));
	SceneGraph::GetInstance()->AddGameObject(BlueDiceObj, tag);
}

