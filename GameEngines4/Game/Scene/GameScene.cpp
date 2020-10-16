#include "GameScene.h"

GameScene::GameScene() : Scene()
{

}

GameScene::~GameScene()
{
}


bool GameScene::OnCreate()
{
	Debug::Info("Creating Game Scene", "GameScene.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCamera(new Camera);
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(0.0f, 1.0f, 4.0f));
	CoreEngine::GetInstance()->GetCamera()->Addlightsources(new LightSource(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.5f));

	AudioHandler::getInstance()->Initialize(CoreEngine::GetInstance()->GetCamera()->GetPosition(), vec3(0), vec3(0, 0, -1), vec3(0,1,0));
	CollisionHandler::GetInstance()->OnCreate(100.f);

	
	for (int i = 0; i < 6; i++)
	{
		CoreEngine::GetInstance()->GetCamera()->AddPlane(new Plane());
	}

	CoreEngine::GetInstance()->GetCamera()->ExtractPlane();

	
	Model * model1 = new Model(
		"./Resources/Models/Dice.obj",
		"./Resources/Materials/Dice.mtl", 
		ShaderHandler::getInstance()->GetShader("basicShader"));

	SceneGraph::GetInstance()->AddModel(model1);

	GameObject* test = new GameObject(model1, vec3(2.0f, 0.0f, -5.0f));
	SceneGraph::GetInstance()->AddGameObject(test, "DICE");
	SceneGraph::GetInstance()->setTarget(test);


	//for (size_t i = 0; i < 10; i++)
	//{
	//	for (size_t j = 0; j < 10; j++)
	//	{
	//		vec3 pos = vec3(0.0f + (i + 2.0f), 0.0f, 0.0f + (j + 2.0f));
	//		GameObject* test = new GameObject(model1, pos);
	//		SceneGraph::GetInstance()->AddGameObject(test, "DICE" + i);
	//	}
	//}		
	
	for (size_t i = 0; i < 2 ; i++)
	{
		vec3 pos = vec3(2.0f + i + 2.0f , 0.0f, -5.0f - i - 2.0f );
		GameObject* test = new GameObject(model1, pos);
		SceneGraph::GetInstance()->AddGameObject(test, "DICE" + i);

		SceneGraph::GetInstance()->setTargetList(test);
	}	

	Model * model2 = new Model(
			"./Resources/Models/Apple.obj",
			"./Resources/Materials/Apple.mtl",
			ShaderHandler::getInstance()->GetShader("basicShader"));

	SceneGraph::GetInstance()->AddModel(model2);

	GameObject* apple = new GameObject(model2, vec3(-1.0f, 0.0f, 0.0f));

	apple->SetScale(glm::vec3(0.2f));

	apple->AddComponent<TestClassA>("test");
	apple->GetComponent<TestClassA>();
	apple->RemoveComponent<TestClassA>();


	SceneGraph::GetInstance()->AddGameObject(apple, "apple");

	SceneGraph::GetInstance()->setCharacter(apple);
	SceneGraph::GetInstance()->setupSeek();
	SceneGraph::GetInstance()->setupArrive();
	SceneGraph::GetInstance()->setupCollisionAvoidance();

	
	GuiObject* gui = new GuiObject(vec2(CoreEngine::GetInstance()->GetWindowSize().x / 2.0f, CoreEngine::GetInstance()->GetWindowSize().y / 2.0f));

	gui->AddComponent<GuiImageComponent>();
	gui->GetComponent<GuiImageComponent>()->OnCreate("sun");

	SceneGraph::GetInstance()->AddGuiObject(gui, "sunGUI");

	backroundSound = new AudioSource("Funky Chill 2 loop.wav");
	backroundSound->OnCreate(nullptr);
	backroundSound->playSound();


	{
	//get from mesh annd obj loader
	/*cout << "Min Vert: " << to_string(model3->GetBoundingBox().minVert) << endl;
	cout << "Max Vert: " << to_string(model3->GetBoundingBox().maxVert) << endl;*/
	
	//cout << "Min Vert: " << to_string(model2->GetBoundingBox().minVert) << endl;
	//cout << "Max Vert: " << to_string(model2->GetBoundingBox().maxVert) << endl;
	
	cout << "PLANE LEFT: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->x, 
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->y,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->z,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->d)) << endl;

	cout << "PLANE RIGHT: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->x, 
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->y,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->z,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->d)) << endl;

	cout << "PLANE TOP: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->x, 
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->y,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->z,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->d)) << endl;

	cout << "PLANE BOTTOM: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->x, 
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->y,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->z,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->d)) << endl;	
	
	cout << "PLANE NEAR: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->x, 
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->y,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->z,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->d)) << endl;
	
	cout << "PLANE FAR: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->x, 
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->y,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->z,
											CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->d)) << endl;

	}

	return true;
}

void GameScene::Update(const float deltaTime_)
{
	//std::cout << deltaTime_ << std::endl;
	//shape->Update(deltaTime_);
	SceneGraph::GetInstance()->Update(deltaTime_);

	if (SceneGraph::GetInstance()->getGuiObject("sunGUI")->isInside(MouseEventListener::GetMousePosition()))
	{
		std::cout << "inside GUI" << std::endl;
	}
	
}

void GameScene::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//shape->Render(CoreEngine::GetInstance()->GetCamera());
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}

void GameScene::Draw()
{
	SceneGraph::GetInstance()->Draw(CoreEngine::GetInstance()->GetCamera());
}
