#include "MainMenu.h"
MainMenu::MainMenu() : check(true), infoDistance(6), Scene()
{

}

MainMenu::~MainMenu()
{
	delete backroundSound;
	backroundSound = nullptr;

	delete emmitter;
	backroundSound = nullptr;
}


bool MainMenu::OnCreate()
{
	Debug::Info("Creating Game Scene", "MainMenu.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCamera(new Camera);
	CoreEngine::GetInstance()->GetCamera()->SetPosition(vec3(5.0f, 5.0f, 0.0f));
	CoreEngine::GetInstance()->GetCamera()->SetRotation(-90, -20);
	CoreEngine::GetInstance()->GetCamera()->Addlightsources(new LightSource(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.8f));

	AudioHandler::getInstance()->Initialize(CoreEngine::GetInstance()->GetCamera()->GetPosition(), vec3(0), vec3(0, 0, -1), vec3(0, 1, 0));

	GuiObject* gui = new GuiObject(vec2(CoreEngine::GetInstance()->GetWindowSize().x / 2, CoreEngine::GetInstance()->GetWindowSize().y / 2));
	gui->AddComponent<GuiImageComponent>();
	gui->GetComponent<GuiImageComponent>()->OnCreate("Background");
	SceneGraph::GetInstance()->AddGuiObject(gui, "Background");

	backroundSound = new AudioSource("Funky Chill 2 loop.wav");
	backroundSound->OnCreate(nullptr);
	backroundSound->playSound();

	GuiObject* startButton = new GuiObject(vec2(720, CoreEngine::GetInstance()->GetWindowSize().y - 400));
	startButton->AddComponent<GuiImageComponent>();
	startButton->GetComponent<GuiImageComponent>()->OnCreate("StartButton");
	SceneGraph::GetInstance()->AddGuiObject(startButton, "StartButton");


	GuiObject* exitButton = new GuiObject(vec2(720, CoreEngine::GetInstance()->GetWindowSize().y - 520));
	exitButton->AddComponent<GuiImageComponent>();
	exitButton->GetComponent<GuiImageComponent>()->OnCreate("ExitButton");
	SceneGraph::GetInstance()->AddGuiObject(exitButton, "ExitButton");



	return true;
}

void MainMenu::Update(const float deltaTime_)
{
	SceneGraph::GetInstance()->Update(deltaTime_);

	if (SceneGraph::GetInstance()->getGuiObject("StartButton")->isInside(MouseEventListener::GetMousePosition()))
	{
		std::cout << "inside GUI" << std::endl;

		bool hit = MouseEventListener::ClickButton();

		if (hit == true)
		{
			
			CoreEngine::GetInstance()->SetCurrentScene(1);
			AudioHandler::getInstance()->OnDestroy();
			//backroundSound->~AudioSource();
		}
	}
}

void MainMenu::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//shape->Render(CoreEngine::GetInstance()->GetCamera());
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
	SceneGraph::GetInstance()->DelayedRender(deltaTime);

	//emmitter->Render(CoreEngine::GetInstance()->GetCamera());
	Draw();
}

void MainMenu::Draw()
{
	SceneGraph::GetInstance()->Draw(CoreEngine::GetInstance()->GetCamera());
}
