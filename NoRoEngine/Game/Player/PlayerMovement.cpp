#include "PlayerMovement.h"


PlayerMovement::PlayerMovement() : Component()
{
}

PlayerMovement::~PlayerMovement()
{
}

bool PlayerMovement::OnCreate(GameObject* parent_)
{
	parent = parent_;
	if (parent->GetComponent<Animator>() != nullptr)
	{
		currentAniComponent = parent->GetComponent<Animator>();
	}	
	
	if (parent->GetComponent<AudioSource>() != nullptr)
	{
		currentAudio = parent->GetComponent<AudioSource>();

	}	
	
	if (parent->GetComponent<Player>() != nullptr)
	{
		playerComponent = parent->GetComponent<Player>();

	}
	target = SceneGraph::GetInstance()->getGameObject("GUN1");

	return true;
}

void PlayerMovement::Update(float deltaTime_)
{
	//CHECK PLAYER INPUT
	input[0] = KeyEventListener::GetKeyState("W");
	input[1] = KeyEventListener::GetKeyState("A");
	input[2] = KeyEventListener::GetKeyState("S");
	input[3] = KeyEventListener::GetKeyState("D");

	characterMovement(deltaTime_, input);

	//if (target != nullptr)
	//{
	//	BoundingBox x = target->GetBoundingBox();
	//	onMove = parent->GetBoundingBox().Intersect(&x);
	//}
	
	//if (onMove == false)
	//{
	//	
	//}
	//else
	//{
	//	cout << "" << endl;
	//}

}

bool PlayerMovement::delayedUpdate()
{
	return false;
}

void PlayerMovement::characterMovement(const float deltatime_, bool inputs[4])
{
	float angle_ = 0.0f;
	vec3 _inputDirection = vec3(0);
	vec3 angelVelocity = vec3(0);
	
	if (currentAniComponent != nullptr)
	{
		if (currentAniComponent->getCurrentAniState() != Anim::IDLE)
		{
			if (currentAniComponent->isDone == true)
			{
				currentAniComponent->runAnimation(0);
				if (playerComponent->isDeath())
				{
					currentAniComponent->isDone = true;
				}
				else
				{
					currentAniComponent->isDone = false;
				}
				
				currentAudio->playSound();
			}
		}
	}

	if (playerComponent != nullptr)
	{
		if (!playerComponent->isDeath())
		{
			if (inputs[0])
			{
				currentAniComponent->runAnimation(1);
				angle_ = glm::radians(180.0f);
				angelVelocity.y = glm::radians(180.0f);
				_inputDirection.z -= 1;
				//_inputDirection.z -= speed * deltatime_;
			}

			if (inputs[1])
			{
				currentAniComponent->runAnimation(1);
				angle_ = glm::radians(-90.0f);
				angelVelocity.y = glm::radians(-90.0f);
				//_inputDirection.x -= speed * deltatime_;
				_inputDirection.x -= 1;
			}

			if (inputs[2])
			{
				currentAniComponent->runAnimation(1);
				angle_ = glm::radians(0.0f);
				angelVelocity.y = glm::radians(0.0f);
				//_inputDirection.z += speed * deltatime_;
				_inputDirection.z += 1;
			}
			if (inputs[3])
			{
				currentAniComponent->runAnimation(1);
				angle_ = glm::radians(90.0f);
				angelVelocity.y = glm::radians(90.0f);
				//_inputDirection.x += speed * deltatime_;
				_inputDirection.x += 1;
			}

			if (parent != nullptr && _inputDirection != vec3(0))
			{
				Move(_inputDirection, angelVelocity, angle_, deltatime_);
			}
		}
		else
		{
			currentAniComponent->runAnimation(3);
		}
	}
	else
	{
		playerComponent = parent->GetComponent<Player>();
	}

	if (currentAniComponent->isDieAnimationDone && playerComponent->isDeath())
	{
		SceneGraph::GetInstance()->RemoveObj(parent->GetTag());
	}
}

void PlayerMovement::Move(vec3 _inputDirection, vec3 angularVelocity, const float time_)
{
	float elapsedTime = (float)SDL_GetTicks() / 1000;
	vec3 forwardVector = Physics::ComputeForwardVector(parent->getQuaternion());

	cout << "X:" << forwardVector.x << " Y:" << forwardVector.y << " Z:" << forwardVector.z << endl;


	vec3 test(1, 0, 0);
	quat q(vec3(0, glm::radians(90.0f), 0));

	quat q1 = normalize(q);

	quat qv(0, 1, 0, 0);

	quat result = q * qv * q1;
	vec3 s = Physics::ComputeForwardVector(result);

	quat df(angularVelocity);

	parent->SetQuaternion(df);


	vec3 newforwardVector = Physics::ComputeForwardVector(parent->getQuaternion());
	normalize(newforwardVector);
	cout << "New forward " << "X:" << newforwardVector.x << " Y:" << newforwardVector.y << " Z:" << newforwardVector.z << endl;

	vec3 pos = parent->GetPosition() + _inputDirection;
	parent->SetPosition(pos);
}

void PlayerMovement::Move(vec3 _inputDirection, vec3 angularVelocity, float angle, const float time_)
{
	quat sdf = angleAxis(angle, vec3(0, 1, 0));
	//parent->SetAngle(angle);

	parent->SetQuaternion(sdf);
	vec3 pos = parent->GetPosition() + _inputDirection;
	parent->SetPosition(pos);
}
