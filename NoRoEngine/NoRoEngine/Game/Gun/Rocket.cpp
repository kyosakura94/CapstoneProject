#include "Rocket.h"


Rocket::Rocket()
{
}

Rocket::~Rocket()
{

}

bool Rocket::OnCreate(GameObject* parent_)
{
	parent = parent_;
	player = SceneGraph::GetInstance()->getGameObject("player");

	if (player != nullptr)
	{
		playerComponent = player->GetComponent<Player>();

	}
	

	return true;
}

void Rocket::Update(float deltaTime_)
{
	parent->SetLifeTime(parent->getLifetime() - deltaTime_);
	MoveForward(deltaTime_);

	if (player != nullptr)
	{
		if (!playerComponent->isDeath())
		{
			BoundingBox x = player->GetBoundingBox();
			bool result = parent->GetBoundingBox().Intersect(&x);

			if (playerComponent != nullptr)
			{
				if (result)
				{
					playerComponent->collideRocket(result, parent->GetTag());
					cout << "hit " << parent->GetTag() << endl;
					SceneGraph::GetInstance()->Remove(parent->GetTag());
				}
			}
		}
	}


}

bool Rocket::delayedUpdate()
{
	return false;
}

void Rocket::TriggerCollider()
{

}

void Rocket::MoveForward(float time_)
{
	//parent->SetAccel(vec3(-5.0f, 0.0f, 0.0f));
	if (parent != nullptr)
	{
		Physics::SimpleNewtonMotion(*parent, time_);
	}
	
}
