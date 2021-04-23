#include "Spin.h"


Spin::Spin()
{
}

Spin::Spin(float spinRate_) : spinRate(360.0f)
{
	spinRate = spinRate_;
}

Spin::~Spin()
{

}

bool Spin::OnCreate(GameObject* parent_)
{
	if (parent_ != nullptr)
	{
		parent = parent_;
	}
	else
	{
		return false;
	}

	return true;
}

void Spin::Update(float deltaTime_)
{
	spinRate -= deltaTime_;
	quat sdf = glm::angleAxis(spinRate, vec3(0, 1, 0));
	//cout << spinRate << endl;
	//cout << to_string(parent->getQuaternion()) << endl;
	parent->SetQuaternion(sdf);
}

bool Spin::delayedUpdate()
{
	return false;
}
