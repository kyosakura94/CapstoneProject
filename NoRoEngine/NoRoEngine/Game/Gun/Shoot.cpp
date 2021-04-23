#include "Shoot.h"
#include "Rocket.h"


Shoot::Shoot()
{
}

Shoot::~Shoot()
{
}

bool Shoot::OnCreate(GameObject* parent_)
{

	parent = parent_;
	player_ = SceneGraph::GetInstance()->getGameObject("player");
	//playerComponent = player_->GetComponent<Player>();
	timeRate = 0.5f;
	return true;

}

void Shoot::Update(float deltaTime_)
{
	if (player_ != nullptr)
	{
		playerComponent = player_->GetComponent<Player>();

		if (playerComponent != nullptr)
		{
			if (!playerComponent->isDeath())
			{
				BoundingBox x = player_->GetBoundingBox();
				bool result = parent->GetCollideBoundingBox().Intersect(&x);

				if (result)
				{
					timeRate -= deltaTime_;
					if (timeRate <= 0)
					{
						fireTrigger();
						timeRate = 0.5f;
					}
				}
				else
				{
					//cout << "Collide false " << endl;
				}
			}
			else
			{
				playerComponent = nullptr;
				player_ = nullptr;
			}
		}

	}
	else
	{
		player_ = SceneGraph::GetInstance()->getGameObject("player");
	}
}

bool Shoot::delayedUpdate()
{
	return false;
}

void Shoot::fireTrigger()
{
	index_++;
	Model* RocketModel = SceneGraph::GetInstance()->getModel("Rocket_1");
	quat sdf;
	vec3 velocity;
	vec3 acc;
	int typeP = 0;
	switch (type)
	{
	case 0:
		sdf = glm::angleAxis(300.0f, vec3(0, 1, 0));
		offset = vec3(-0.5f, 1.8f, 0);
		velocity = vec3(-12, 0, 0);
		acc = vec3(-5, 0, 0);
		typeP = 1;
		break;
	case 1:
		sdf = quat(0, 0, 1, 0);
		offset = vec3(0, 1.8f, -0.5f);
		velocity = vec3(0, 0, -12.0f);
		typeP = 2;
		break;
	default:
		break;
	}


	GameObject* RocketObj = new GameObject(RocketModel, parent->GetPosition() + offset , sdf);
	string tag = "Rocket";
	tag.append(std::to_string(index_));
	SceneGraph::GetInstance()->AddGameObject(RocketObj, tag);

	RocketObj->SetVelocity(velocity);
	RocketObj->SetAccel(acc);
	RocketObj->SetLifeTime(2.0f);
	RocketObj->AddComponent<Rocket>();
	RocketObj->AddComponent<AudioSource>("Hit.wav", false);
	RocketObj->GetComponent<AudioSource>()->setVolume(0.2f);
	RocketObj->GetComponent<AudioSource>()->playSound();
	


	RocketObj->AddComponent<ParticleEmitter>();
	ParticleEmitter* partice = RocketObj->GetComponent<ParticleEmitter>();

	partice->Init(20, "particleShader", 2.0f);
	partice->setType(typeP);
	partice->setRandom(true);
	partice->setColorRange(1, 0.2, 0);
}
