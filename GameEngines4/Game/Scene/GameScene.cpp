#include "GameScene.h"

GameScene::GameScene() : Scene()
{

}

GameScene::~GameScene()
{
	delete backroundSound;
	backroundSound = nullptr;

	delete emmitter;
	backroundSound = nullptr;
}


bool GameScene::OnCreate()
{
	Debug::Info("Creating Game Scene", "GameScene.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCamera(new Camera);
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(5.0f, 5.0f, 0.0f));
	CoreEngine::GetInstance()->GetCamera()->SetRotation(-90, -45);
	CoreEngine::GetInstance()->GetCamera()->Addlightsources(new LightSource(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.8f));

	AudioHandler::getInstance()->Initialize(CoreEngine::GetInstance()->GetCamera()->GetPosition(), vec3(0), vec3(0, 0, -1), vec3(0,1,0));

	CollisionHandler::GetInstance()->OnCreate(100.f);

	bool result =  GJKCheck->CollisonCheck();

	grid.OnCreate(6,6,10,vec3(0,0,0));

	//grid->GetGridObject(1, 0);

	for (int i = 0; i < 6; i++)
	{
		CoreEngine::GetInstance()->GetCamera()->AddPlane(new Plane());
	}

	CoreEngine::GetInstance()->GetCamera()->ExtractPlane();

	
	Model * model1 = new Model(
		"./Resources/Models/Dice.obj",
		"./Resources/Materials/Dice.mtl", 
		ShaderHandler::getInstance()->GetShader("basicShader"),
		CoreEngine::GetInstance()->getRendererType());

	Model * model3 = new Model(
		"./Resources/Models/Dice1.obj",
		"./Resources/Materials/Dice1.mtl", 
		ShaderHandler::getInstance()->GetShader("basicShader"),
		CoreEngine::GetInstance()->getRendererType());



	SceneGraph::GetInstance()->AddModel(model1);
	SceneGraph::GetInstance()->AddModel(model3);

	GameObject* test = new GameObject(model1, vec3(2.0f, 0.0f, -5.0f));
	SceneGraph::GetInstance()->AddGameObject(test, "DICE");
	SceneGraph::GetInstance()->setTarget(test);

	test->AddComponent<AudioSource>("enemy_fire.wav", false);

	for (size_t x = 0; x < grid.GetHeight(); x++)
	{
		vec3 offsetX(x,0,0);

		for (size_t y = 0; y < grid.GetWidth(); y++)
		{
			vec3 offsetY(0,0,y);
			vec3 pos = vec3(x + 2, 0.0f, y + 2) + offsetX + offsetY;


			if (x < (grid.GetHeight() - 1) && y == 1)
			{
				GameObject* test = new GameObject(model3, pos);
				string tag = "DICE";
				tag.append(std::to_string(x));
				tag.append(std::to_string(y));
				SceneGraph::GetInstance()->AddGameObject(test, tag);

				grid.setObj(x, y, test);
				grid.setWalkable(x, y, false);
			}
			else
			{
				GameObject* test = new GameObject(model1, pos);
				string tag = "DICE";
				tag.append(std::to_string(x));
				tag.append(std::to_string(y));
				SceneGraph::GetInstance()->AddGameObject(test, tag);

				grid.setObj(x, y, test);
				grid.setWalkable(x, y, true);
			}

			//else if (y < (grid.GetHeight() - 1) && x == 2)
			//{
			//	//GameObject* test = new GameObject(model3, pos);
			//	//string tag = "DICE";
			//	//tag.append(std::to_string(x));
			//	//tag.append(std::to_string(y));
			//	//SceneGraph::GetInstance()->AddGameObject(test, tag);

			//	//grid.setObj(x, y, test);
			//	//grid.setWalkable(x, y, false);
			//}

		}
	}		
	
	//emmitter = new ParticleEmitter(10, "particleShader");
	//for (size_t i = 0; i < 2 ; i++)
	//{
	//	vec3 pos = vec3(2.0f + i + 2.0f , 0.0f, -5.0f - i - 2.0f );
	//	GameObject* test = new GameObject(model1, pos);
	//	SceneGraph::GetInstance()->AddGameObject(test, "DICE" + i);

	//	SceneGraph::GetInstance()->setTargetList(test);
	//}	

	Model * model2 = new Model(
			"./Resources/Models/Apple.obj",
			"./Resources/Materials/Apple.mtl",
			ShaderHandler::getInstance()->GetShader("basicShader"),
			CoreEngine::GetInstance()->getRendererType());

	SceneGraph::GetInstance()->AddModel(model2);

	GameObject* apple = new GameObject(model2, vec3(-1.0f, 0.0f, 0.0f));

	apple->SetScale(glm::vec3(0.2f));

	apple->AddComponent<TestClassA>("test");
	apple->GetComponent<TestClassA>();
	apple->RemoveComponent<TestClassA>();

	apple->AddComponent<ParticleEmitter>(10, "particleShader");


	SceneGraph::GetInstance()->AddGameObject(apple, "apple");

	SceneGraph::GetInstance()->setCharacter(apple);
	SceneGraph::GetInstance()->setupSeek();
	SceneGraph::GetInstance()->setupArrive();

	SceneGraph::GetInstance()->setupCollisionAvoidance();

	//emmitter->Init(10, "particleShader");
	
	GuiObject* gui = new GuiObject(vec2(50, CoreEngine::GetInstance()->GetWindowSize().y - 50));

	gui->AddComponent<GuiImageComponent>();
	gui->GetComponent<GuiImageComponent>()->OnCreate("sun");
	

	SceneGraph::GetInstance()->AddGuiObject(gui, "sunGUI");

	backroundSound = new AudioSource("Funky Chill 2 loop.wav");
	backroundSound->OnCreate(nullptr);
	//backroundSound->playSound();


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
	deltaTime = deltaTime_;
	//SceneGraph::GetInstance()->UpdateClick(deltaTime_, grid, e_);
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
	SceneGraph::GetInstance()->DelayedRender(deltaTime);

	//emmitter->Render(CoreEngine::GetInstance()->GetCamera());
}

void GameScene::Draw()
{
	SceneGraph::GetInstance()->Draw(CoreEngine::GetInstance()->GetCamera());
}
