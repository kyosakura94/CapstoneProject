#include "Player.h"
#include "../../Engine/Rendering/2D/GuiImageComponent.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::OnCreate(GameObject* parent_)
{
	originalPosition = vec3(2, 1, 0);
	backroundSound = new AudioSource("Lose.wav", false);
	backroundSound = new AudioSource("Win.wav", false);
	backroundSound->OnCreate(nullptr);

	float offset = 50.0f;
	for (size_t i = 0; i < lifeCount; i++)
	{
		GuiObject* gui = new GuiObject(vec2(offset * (i + 1), CoreEngine::GetInstance()->GetWindowSize().y - 50));
		string tag = "Heart";
		tag.append(std::to_string(i));
		gui->AddComponent<GuiImageComponent>();
		gui->GetComponent<GuiImageComponent>()->OnCreate("heart");

		SceneGraph::GetInstance()->AddGuiObject(gui, tag);
	}

	parent = parent_;
	return true;
}

void Player::Update(float deltaTime_)
{
	if (!isDie)
	{
		GameObject* target = SceneGraph::GetInstance()->getGameObject("apple");

		//CHECK IF PLAYER COLLIDE TO ANY BOUNDING BOX

		if (target != nullptr)
		{
			BoundingBox x = target->GetCollideBoundingBox();

			bool result = parent->GetBoundingBox().Intersect(&x);
			if (KeyEventListener::GetKeyState("Q") && result)
			{
				target->GetComponent<AudioSource>()->playSound();
				cout << "Collide" << endl;
				SceneGraph::GetInstance()->Remove(target->GetTag());

				GuiObject* gui = new GuiObject(vec2(CoreEngine::GetInstance()->GetWindowSize().x - 50, CoreEngine::GetInstance()->GetWindowSize().y - 50));

				gui->AddComponent<GuiImageComponent>();
				gui->GetComponent<GuiImageComponent>()->OnCreate("star");

				SceneGraph::GetInstance()->AddGuiObject(gui, "starGUI");
				appleCollect = true;
			}
		}

		if (hitbyRocket == true)
		{
			string tag = "Heart";
			parent->GetComponent<AudioSource>()->playSound("Hurt.mp3");
			tag.append(std::to_string(lifeCount - 1));

			lifeCount--;
			
			SceneGraph::GetInstance()->RemoveGUI(tag);
			hitbyRocket = false;
		}

		if (lifeCount >= 1)
		{
			vec3 diff = originalPosition - parent->GetPosition();

			float distance = sqrtf(dot(diff, diff));

			if (appleCollect && distance <= 0.2f)
			{
				if (isFirsttime == true)
				{
					backroundSound->playSound("Win.wav");
					isFirsttime = false;
				}

				UISetUP("LevelCompete", "next");
				cout << "Game End" << endl;
				
			}
		}
		else
		{
			isDie = true;	
			if (isFirsttime == true)
			{
				backroundSound->playSound("Lose.wav");
				isFirsttime = false;
			}
			UISetUP("LevelFail", "again");
			cout << "You die" << endl;
		}
	}
	else
	{
		
	}
}

bool Player::delayedUpdate()
{
	return false;
}

void Player::collideRocket(bool hit_, string tag_)
{
	hittag = tag_;
	hitbyRocket = hit_;
}

void Player::UISetUP(string ds, string dk_)
{
	GuiObject* gui = new GuiObject(vec2(CoreEngine::GetInstance()->GetWindowSize().x / 2, CoreEngine::GetInstance()->GetWindowSize().y / 2));

	gui->AddComponent<GuiImageComponent>();
	gui->GetComponent<GuiImageComponent>()->OnCreate(ds);

	SceneGraph::GetInstance()->AddGuiObject(gui, ds);

	GuiObject* asd = new GuiObject(vec2(CoreEngine::GetInstance()->GetWindowSize().x / 2 - 60, (CoreEngine::GetInstance()->GetWindowSize().y / 2) - 100));

	asd->AddComponent<GuiImageComponent>();
	asd->GetComponent<GuiImageComponent>()->OnCreate("setting");

	SceneGraph::GetInstance()->AddGuiObject(asd, "setting");

	GuiObject* asdw = new GuiObject(vec2(CoreEngine::GetInstance()->GetWindowSize().x / 2 + 60, (CoreEngine::GetInstance()->GetWindowSize().y / 2) - 100));

	asdw->AddComponent<GuiImageComponent>();
	asdw->GetComponent<GuiImageComponent>()->OnCreate(dk_);

	SceneGraph::GetInstance()->AddGuiObject(asdw, dk_);
}
