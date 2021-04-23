#include "GameScene.h"
#include "../Player/Spin.h"

GameScene::GameScene() : check(true), infoDistance(6), Scene()
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
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(10.0f, 23.0f, 24.0f));
	CoreEngine::GetInstance()->GetCamera()->SetRotation(-90, -59);
	CoreEngine::GetInstance()->GetCamera()->Addlightsources(new LightSource(glm::vec3(00.0f, 5.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.15f, 0.9f));

	AudioHandler::getInstance()->Initialize(CoreEngine::GetInstance()->GetCamera()->GetPosition(), vec3(0), vec3(0, 0, -1), vec3(0,1,0));
	CollisionHandler::GetInstance()->OnCreate(100.f);

	createShadowMap();
	setUpEnvironment();
	setUpPlayer();
	//setUpUI();
	setUpSound();

	return true;
}

void GameScene::Update(const float deltaTime_)
{


	//SceneGraph::GetInstance()->RPGPlayerMove(deltaTime_, "animatedObj", input);

	deltaTime = deltaTime_;

	vec3 targetPosition(SceneGraph::GetInstance()->getGameObject("DICE")->GetPosition());

	GameObject* target = SceneGraph::GetInstance()->getGameObject("apple");

	if (target!= nullptr)
	{
		vec3 diff = targetPosition - target->GetPosition();
		float distance = sqrtf(dot(diff, diff));
		ConditionManager::GetInstance()->getConditon("floatIdle")->setResult(ConditionManager::GetInstance()->getDistance(), distance);
	}



	//bool result = GJKCheck->CollisonCheck(SceneGraph::GetInstance()->getGameObject("DICE"), SceneGraph::GetInstance()->getGameObject("apple"));
	//
	//if (result == true && check == true)
	//{
	//	ConditionManager::GetInstance()->getConditon("floatAttack")->setResult(6, 5);
	//	ConditionManager::GetInstance()->setDistance(0);
	//	//check = false;
	//}


	//if (result == true)
	//{
	//	Condition::GetInstance()->setInside(true);
	//	Condition::GetInstance()->setOutside(false);
	//}
	//else
	//{
	//	Condition::GetInstance()->setOutside(true);
	//	Condition::GetInstance()->setInside(true);
	//}

	//normal update of all gameobject in the scene

	//CHECK IF PLAYER IS DEATH OR NOT


	SceneGraph::GetInstance()->Update(deltaTime_);




	

	//update for the A* algrothim
	//SceneGraph::GetInstance()->UpdateClick(deltaTime_, grid, e_);

	//GuiObject *sunObj = SceneGraph::GetInstance()->getGuiObject("sunGUI");
	//if (sunObj != nullptr && sunObj->isInside(MouseEventListener::GetMousePosition()))
	//{
	//	std::cout << "inside GUI" << std::endl;
	//	
	//	bool hit = MouseEventListener::ClickButton();

	//	if (hit == true)
	//	{
	//		CoreEngine::GetInstance()->SetCurrentScene(3);
	//	}
	//}

	//REMOVE ALL THE ROCKET 

	//for (size_t i = 0; i < 4; i++)
	//{
	//	input[i] = false;
	//}

	if (SceneGraph::GetInstance()->getGuiObject("again"))
	{
		if (SceneGraph::GetInstance()->getGuiObject("again")->isInside(MouseEventListener::GetMousePosition()))
		{
			std::cout << "inside GUI" << std::endl;

			bool hit = MouseEventListener::ClickButton();

			if (hit == true)
			{
				CoreEngine::GetInstance()->SetCurrentScene(4);
				AudioHandler::getInstance()->OnDestroy();
			}
		}
	}	
	
	if (SceneGraph::GetInstance()->getGuiObject("setting"))
	{
		if (SceneGraph::GetInstance()->getGuiObject("setting")->isInside(MouseEventListener::GetMousePosition()))
		{
			std::cout << "inside GUI" << std::endl;

			bool hit = MouseEventListener::ClickButton();

			if (hit == true)
			{
				CoreEngine::GetInstance()->SetCurrentScene(3);
				AudioHandler::getInstance()->OnDestroy();
			}
		}
	}	
	
	if (SceneGraph::GetInstance()->getGuiObject("next"))
	{
		
		if (SceneGraph::GetInstance()->getGuiObject("next")->isInside(MouseEventListener::GetMousePosition()))
		{
			std::cout << "inside GUI" << std::endl;

			bool hit = MouseEventListener::ClickButton();

			if (hit == true)
			{
				CoreEngine::GetInstance()->SetCurrentScene(3);
				AudioHandler::getInstance()->OnDestroy();
			}
		}
	}
	
}

void GameScene::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//shape->Render(CoreEngine::GetInstance()->GetCamera());
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera(), depthMapFBO, depthMap);
	SceneGraph::GetInstance()->DelayedRender(deltaTime);
	//emmitter->Render(CoreEngine::GetInstance()->GetCamera());
	Draw();
}

void GameScene::Draw()
{
	SceneGraph::GetInstance()->Draw(CoreEngine::GetInstance()->GetCamera());
}

void GameScene::CreatePlayer(vec3 pos, string modelName, string tag)
{
}

void GameScene::CreatePlayer(string json_)
{

}

void GameScene::createShadowMap()
{
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GameScene::setUpPlayer()
{
	AnimatedModel* animatedmodel = new AnimatedModel(
		"./Resources/Models/Character/Baqir_Idle_2015_27.dae",
		"./Resources/Materials/Baqir_Idle_2015_27.mtl",
		ShaderHandler::getInstance()->GetShader("animatedShader"),
		CoreEngine::GetInstance()->getRendererType());

	SceneGraph::GetInstance()->AddModel(animatedmodel);

	player = new GameObject(animatedmodel, vec3(2, 1, 0));
	player->SetScale(glm::vec3(0.4f));
	player->AddComponent<Animator>();
	player->AddComponent<Player>();
	player->AddComponent<AudioSource>("walk.mp3", false);
	player->GetComponent<AudioSource>()->Init("Hurt.mp3", false);
	player->GetComponent<AudioSource>()->setVolume(0.5f);

	player->AddComponent<PlayerMovement>();
	//Add Animation to the list
	Animation_B* idle = new Animation_B();
	idle->loadAnimation("./Resources/Models/Character/Baqir_Idle_2015_27.dae");
	player->GetComponent<Animator>()->pushAnimation(idle);

	Animation_B* idle1 = new Animation_B();
	idle1->loadAnimation("./Resources/Models/Character/Baqir_Idle_2015_28.dae");
	player->GetComponent<Animator>()->pushAnimation(idle1);

	Animation_B* run = new Animation_B();
	run->loadAnimation("./Resources/Models/Character/Bq_Test_7.dae");
	player->GetComponent<Animator>()->pushAnimation(run);


	Animation_B* die = new Animation_B();
	die->loadAnimation("./Resources/Models/Character/Die.dae");
	player->GetComponent<Animator>()->pushAnimation(die);

	//Play default animation first time
	player->GetComponent<Animator>()->doAnimation(idle);

	SceneGraph::GetInstance()->AddGameObject(player, "player");

	if (player->GetComponent<Player>() != nullptr)
	{
		playerComponent = player->GetComponent<Player>();

	}
}

void GameScene::setUpEnvironment()
{
	//CREATE PLANE-GROUND
	AnimatedModel* plane = new AnimatedModel("./Resources/Models/Environment/Plane.dae", "./Resources/Materials/Plane.mtl", ShaderHandler::getInstance()->GetShader("basicShader"), CoreEngine::GetInstance()->getRendererType());
	SceneGraph::GetInstance()->AddModel(plane);
	GameObject* planeObj = new GameObject(plane, vec3(10.0f, 0.0f, 0.0f));
	planeObj->SetScale(vec3(17.0f));

	//////////////////////////////////////////
	//CREATE TITLES

	AnimatedModel* grassTitleModel = new AnimatedModel("./Resources/Models/Environment/Grass_2.dae", "./Resources/Materials/Grass_2.mtl", ShaderHandler::getInstance()->GetShader("basicShader"), CoreEngine::GetInstance()->getRendererType());
	SceneGraph::GetInstance()->AddModel(grassTitleModel);
	GameObject* grassTitleObj = new GameObject(grassTitleModel, vec3(2.0f, 0.00f, 0.0f));
	SceneGraph::GetInstance()->AddGameObject(grassTitleObj, "DICE");
	SceneGraph::GetInstance()->setTarget(grassTitleObj);
	grassTitleObj->AddComponent<AudioSource>("enemy_fire.wav", false);

	//////////////////////////////////////////
	//CREATE GUN Objects
	AnimatedModel* GunModel = new AnimatedModel("./Resources/Models/Environment/Gun_1.dae", "./Resources/Materials/Gun_1.mtl", ShaderHandler::getInstance()->GetShader("basicShader"), CoreEngine::GetInstance()->getRendererType());
	SceneGraph::GetInstance()->AddModel(GunModel);

	quat qua = glm::angleAxis(300.0f, vec3(0, 1, 0));

	//CREATE GUN1
	GameObject* GunObj = new GameObject(GunModel, vec3(20.0f, 1.0f, 4.0f), qua);
	SceneGraph::GetInstance()->AddGameObject(GunObj, "GUN1");
	GunObj->AddComponent<Shoot>();
	GunObj->increaseBB(vec3(26, 10, 1.0f));
	GunObj->SetScale(glm::vec3(1.0f));

	//CREATE GUN1
	GameObject* Gun1Obj = new GameObject(GunModel, vec3(20.0f, 1.0f, 10.0f), qua);
	SceneGraph::GetInstance()->AddGameObject(Gun1Obj, "GUN2");
	Gun1Obj->AddComponent<Shoot>();
	Gun1Obj->increaseBB(vec3(26, 10, 1.0f));
	Gun1Obj->SetScale(glm::vec3(1.0f));

	//CREATE GUN1
	GameObject* Gun11Obj = new GameObject(GunModel, vec3(20.0f, 1.0f, 16.0f), qua);
	SceneGraph::GetInstance()->AddGameObject(Gun11Obj, "GUN3");
	Gun11Obj->AddComponent<Shoot>();
	Gun11Obj->increaseBB(vec3(26, 10, 1.0f));
	Gun11Obj->SetScale(glm::vec3(1.0f));

	//CREATE GUN1
	quat qua1 = quat(0,0,1,0);
	GameObject* Gun4Obj = new GameObject(GunModel, vec3(2.0f, 1.0f, 20.0f), qua1);
	SceneGraph::GetInstance()->AddGameObject(Gun4Obj, "GUN4");
	Gun4Obj->AddComponent<Shoot>();
	Gun4Obj->GetComponent<Shoot>()->setType(1);
	Gun4Obj->increaseBBZ(vec3(1.0f, 10, 28.f));
	Gun4Obj->SetScale(vec3(1.0f));

	//CREATE GUN1
	GameObject* Gun5Obj = new GameObject(GunModel, vec3(8.0f, 1.0f, 20.0f), qua1);
	SceneGraph::GetInstance()->AddGameObject(Gun5Obj, "GUN5");
	Gun5Obj->AddComponent<Shoot>();
	Gun5Obj->GetComponent<Shoot>()->setType(1);
	Gun5Obj->increaseBBZ(vec3(1.0f, 10, 28.f));
	Gun5Obj->SetScale(vec3(1.0f));	
	
	GameObject* Gun6Obj = new GameObject(GunModel, vec3(14.0f, 1.0f, 20.0f), qua1);
	SceneGraph::GetInstance()->AddGameObject(Gun6Obj, "GUN6");
	Gun6Obj->AddComponent<Shoot>();
	Gun6Obj->GetComponent<Shoot>()->setType(1);
	Gun6Obj->increaseBBZ(vec3(1.0f, 10, 28.f));
	Gun6Obj->SetScale(vec3(1.0f));




	//GunObj->AddComponent<Spin>(360.0f);	
	
	//////////////////////////////////////////
	//CREATE Rocket Objects
	AnimatedModel* RocketModel = new AnimatedModel("./Resources/Models/Environment/Rocket_1.dae", "./Resources/Materials/Rocket_1.mtl", ShaderHandler::getInstance()->GetShader("basicShader"), CoreEngine::GetInstance()->getRendererType());
	SceneGraph::GetInstance()->AddModel(RocketModel);

	/*GameObject* RocketObj = new GameObject(RocketModel, vec3(8.0f, 1.5f, 4.0f), sdf);
	SceneGraph::GetInstance()->AddGameObject(RocketObj, "Rocket_1");
*/


	//GunObj->AddComponent<Spin>(360.0f);


	//////////////////////////////////////////
	//CREATE TargetObj

	AnimatedModel* appleModel = new AnimatedModel(
		"./Resources/Models/Apple.obj",
		"./Resources/Materials/Apple.mtl",
		ShaderHandler::getInstance()->GetShader("basicShader"),
		CoreEngine::GetInstance()->getRendererType(), true);

	SceneGraph::GetInstance()->AddModel(appleModel);
	GameObject* apple = new GameObject(appleModel, vec3(20, 1, 20));
	SceneGraph::GetInstance()->AddGameObject(apple, "apple");

	apple->increaseBB(vec3(1.5f));
	apple->SetScale(glm::vec3(0.5f));
	apple->SetVelocity(glm::vec3(0,2.5f,0));
	apple->AddComponent<StateMachine>();
	apple->AddComponent<Spin>(360.0f);
	apple->AddComponent<ParticleEmitter>();
	apple->AddComponent<AudioSource>("collect.wav", false);

	apple->GetComponent<ParticleEmitter>()->Init(20, "particleShader", 2.0f);
	apple->GetComponent<ParticleEmitter>()->setRandom(true);
	apple->GetComponent<ParticleEmitter>()->setColorRange(1,0.2,0);



	//apple->AddComponent<TestClassA>("test");
	//apple->GetComponent<TestClassA>();
	//apple->RemoveComponent<TestClassA>();


	//CRETAE GRID TITLE
	grid.OnCreate(10, 10, 11, vec3(-2, 0, 0));

	//grid->GetGridObject(1, 0);

	for (size_t x = 0; x < grid.GetHeight(); x++)
	{
		vec3 offsetX(x, 0, 0);
		if (x % 2 == 0)
		{
			offsetX = vec3(x, 0, 0);
		}
		else
		{
			offsetX = vec3(x, -0.2, 0);
		}

		for (size_t y = 0; y < grid.GetWidth(); y++)
		{
			vec3 offsetY(0, 0, 0);
			if (y%2 == 0)
			{
				offsetY = vec3(0, 0, y);
			}
			else
			{
				offsetY = vec3(0, -0.2, y);
			}
			
			vec3 pos = vec3(x + 2, 0.0f, y + 2) + offsetX + offsetY;
			GameObject* test = new GameObject(grassTitleModel, pos);
			string tag = "DICE";
			tag.append(std::to_string(x));
			tag.append(std::to_string(y));
			SceneGraph::GetInstance()->AddGameObject(test, tag);

			grid.setObj(x, y, test);
			grid.setWalkable(x, y, true);

		}
	}

	SceneGraph::GetInstance()->setGrid(grid);
}

void GameScene::setUpUI()
{
	GuiObject* gui = new GuiObject(vec2(50, CoreEngine::GetInstance()->GetWindowSize().y - 50));

	gui->AddComponent<GuiImageComponent>();
	gui->GetComponent<GuiImageComponent>()->OnCreate("sun");

	SceneGraph::GetInstance()->AddGuiObject(gui, "sunGUI");

}

void GameScene::setUpSound()
{
	//////////////////////////////////////////
	//CREATE SOUND 

	backroundSound = new AudioSource("055.wav", true);
	backroundSound->OnCreate(nullptr);
	backroundSound->setVolume(0.2f);
	backroundSound->playSound();
}

/*

	//{
	////get from mesh annd obj loader
	///*cout << "Min Vert: " << to_string(model3->GetBoundingBox().minVert) << endl;
	//cout << "Max Vert: " << to_string(model3->GetBoundingBox().maxVert) << endl;
	//
	////cout << "Min Vert: " << to_string(model2->GetBoundingBox().minVert) << endl;
	////cout << "Max Vert: " << to_string(model2->GetBoundingBox().maxVert) << endl;
	//
	//cout << "PLANE LEFT: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->x, 
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->y,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->z,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[0]->d)) << endl;

	//cout << "PLANE RIGHT: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->x, 
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->y,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->z,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[1]->d)) << endl;

	//cout << "PLANE TOP: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->x, 
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->y,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->z,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[2]->d)) << endl;

	//cout << "PLANE BOTTOM: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->x, 
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->y,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->z,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[3]->d)) << endl;	
	//
	//cout << "PLANE NEAR: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->x, 
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->y,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->z,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[4]->d)) << endl;
	//
	//cout << "PLANE FAR: " << to_string(vec4( CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->x, 
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->y,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->z,
	//										CoreEngine::GetInstance()->GetCamera()->GetPlane()[5]->d)) << endl;

	//} 
	
*/

//emmitter = new ParticleEmitter(10, "particleShader");
//for (size_t i = 0; i < 2 ; i++)
//{
//	vec3 pos = vec3(2.0f + i + 2.0f , 0.0f, -5.0f - i - 2.0f );
//	GameObject* test = new GameObject(model1, pos);
//	SceneGraph::GetInstance()->AddGameObject(test, "DICE" + i);

//	SceneGraph::GetInstance()->setTargetList(test);
//}	

/*
	for (int i = 0; i < 6; i++)
	{
		CoreEngine::GetInstance()->GetCamera()->AddPlane(new Plane());
	}

	CoreEngine::GetInstance()->GetCamera()->ExtractPlane();


	for (auto m : diceObj->getModel()->getvertercies(diceObj->getModelIntances()))
	{
		//cout << "Vertecies" << to_string(m) << endl;
	}
*/

/*
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
*/

/*

	//SceneGraph::GetInstance()->setCharacter(apple);
	//SceneGraph::GetInstance()->setupSeek();
	//SceneGraph::GetInstance()->setupArrive();
	//SceneGraph::GetInstance()->setupCollisionAvoidance();

	//bool result = GJKCheck->CollisonCheck(apple, diceObj);
	//emmitter->Init(10, "particleShader");

	//AnimatedModel* redPandaModel = new AnimatedModel(
	//	"./Resources/Models/Character/Baqir_Idle_2015_28.dae",
	//	"./Resources/Materials/RedPanda.mtl",
	//	ShaderHandler::getInstance()->GetShader("animatedShader"),
	//	CoreEngine::GetInstance()->getRendererType());

	//SceneGraph::GetInstance()->AddModel(redPandaModel);

	//GameObject* redPandaObj = new GameObject(redPandaModel, vec3(3, 0, -5));

	//redPandaObj->SetScale(glm::vec3(0.0005f));


	//redPandaObj->AddComponent<Animator>();

	//Animation_B * redPanda_idle = new Animation_B();
	//redPanda_idle->loadAnimation("./Resources/Models/Character/Baqir_Idle_2015_28.dae");

	//redPandaObj->GetComponent<Animator>()->doAnimation(redPanda_idle);



	//SceneGraph::GetInstance()->AddGameObject(redPandaObj, "redPandaObj");

	/////////////////////////////////
	//CREATE UI OBJECTS

	//CollisionResponse* collidie = new CollisionResponse();
*/

/*
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
*/