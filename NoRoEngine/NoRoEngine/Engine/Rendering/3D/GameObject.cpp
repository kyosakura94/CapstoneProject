#include "GameObject.h"

GameObject::GameObject(AnimatedModel* model_, glm::vec3 position_, glm::quat quaternion_)
{
	model = model_;
	position = position_;
	rotation = vec3(0.0f, 1.0f, 0.0f);
	scale = vec3(1.0f);
	angle = 0.0f;
	quaternion = quaternion_;
	accel = vec3(1, 0, 0);
	lifeTime = 100.f;
	hit = false;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale, quaternion);
		box = model->GetBoundingBox();
		collidebox = model->GetBoundingBox();

		box.transform = model->GetTransform(modelInstance);
		collidebox.transform = model->GetTransform(modelInstance);
	}
}

GameObject::GameObject(Model* model_, glm::vec3 position_, glm::quat quaternion_)
{
	model = model_;
	position = position_;
	rotation = vec3(0.0f, 1.0f, 0.0f);
	scale = vec3(1.0f);
	angle = 0.0f;
	quaternion = quaternion_;
	accel = vec3(1, 0, 0);
	lifeTime = 100.f;
	hit = false;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale, quaternion);
		box = model->GetBoundingBox();
		collidebox = model->GetBoundingBox();

		box.transform = model->GetTransform(modelInstance);
		collidebox.transform = model->GetTransform(modelInstance);
	}
}

GameObject::GameObject(Model *model_, vec3 position_) : model(nullptr)
{
	model = model_;
	position = position_;
	rotation = vec3(0.0f, 1.0f, 0.0f);
	scale = vec3(1.0f);
	angle = 0.0f;
	quaternion = quat(0, 0, 0, 0);
	accel = vec3(1,0,0);
	lifeTime = 100.f;
	hit = false;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale, quaternion);
		box = model->GetBoundingBox();
		box.transform = model->GetTransform(modelInstance);
	}
}


GameObject::~GameObject()
{
	for (size_t i = 0; i < componentContainer.size(); i++)
	{
		if (componentContainer[i])
		{
			delete componentContainer[i];
			componentContainer[i] = nullptr;
		}
	}

	model = nullptr;
}

void GameObject::Destroy()
{
	for (size_t i = 0; i < componentContainer.size(); i++)
	{
		if (componentContainer[i])
		{
			delete componentContainer[i];
			componentContainer[i] = nullptr;
		}
	}	
	
	for (size_t i = 0; i < delayComponent.size(); i++)
	{
		if (delayComponent[i])
		{
			delete delayComponent[i];
			delayComponent[i] = nullptr;
		}
	}

	model = nullptr;
}

void GameObject::Render(Camera *camera_)
{
	if (model)
	{
		model->Render(camera_, tag);
	}
}

void GameObject::Update(const float deltaTime_)
{
	//MoveTest(deltaTime_);
	//SetAngle(angle - 0.008f);


	for (size_t i = 0; i < componentContainer.size(); i++)
	{
		if (componentContainer[i] != nullptr)
		{
			componentContainer[i]->Update(deltaTime_);
		}
	}
}

void GameObject::Update(SteeringOutput steering, const float deltaTime_)
{
	
	SetAngle(angle - 0.008f);
	Move(steering, deltaTime_);

	for (size_t i = 0; i < componentContainer.size(); i++)
	{
		componentContainer[i]->Update(deltaTime_);
	}
}
glm::vec3 GameObject::GetPosition() const
{
	return position;
}

glm::vec3 GameObject::GetRotation() const
{
	return rotation;
}

glm::vec3 GameObject::GetScale() const
{
	return scale;
}

glm::vec3 GameObject::GetVelocity()
{
	return velocity;
}

bool GameObject::GetHit() const
{
	return hit;
}

bool GameObject::GetDelayUpdate()
{
	return delayedUpdate;
}

string GameObject::GetTag() const
{
	return tag;
}

float GameObject::GetAngle() const
{
	return angle;
}

vec3 GameObject::GetAccel()
{
	return accel;
}

vec3 GameObject::GetAngVel()
{
	return angVel;
}

quat GameObject::getQuaternion()
{
	return quaternion;
}

void GameObject::SetPosition(glm::vec3 position_)
{
	position = position_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);
		setBoxTransform();
	}
}

void GameObject::SetLifeTime(float life_)
{
	lifeTime = life_;
}

void GameObject::SetRotation(glm::vec3 rotation_)
{
	rotation = rotation_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);
		setBoxTransform();
	}
}

void GameObject::SetScale(glm::vec3 scale_)
{
	scale = scale_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);

		box.transform = model->GetTransform(modelInstance);
		//setBoxTransform();
		box.minVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
		box.maxVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
	}
}

void GameObject::SetVelocity(glm::vec3 Velocity_)
{
	velocity = Velocity_;
}

void GameObject::SetAngle(float angle_)
{
	angle = angle_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);
		setBoxTransform();
	}
}

void GameObject::Move(SteeringOutput steering, const float deltaTime_)
{
	//Update AngluarVelocity
	vec3 upvector(0.0f, 1.0f, 0.0f);

	//angVel = cross(velocity, upvector);
	//
	//angVel = normalize(angVel);

	//angVel = angVel * (velocity.length() / radius);

	//w = 45 * Math.PI / 180

	float maxSpeed = 5.0;
	position += velocity * deltaTime_;
	orientation += rotation * deltaTime_;

	Physics::SimpleNewtonMotion(*this, deltaTime_);

	velocity += steering.getLinear() * deltaTime_;
	rotation += steering.getAngular() * deltaTime_;

	if (velocity.length() > maxSpeed) 
	{
		velocity = normalize(velocity);
		velocity *= maxSpeed;
	}

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);
		setBoxTransform();
	}
}

//void GameObject::MoveTest(SteeringOutput steering, const float deltaTime_)
//{
//	vec3 upvector(0.0f, 1.0f, 0.0f);
//
//	angVel = cross(velocity, upvector);
//	angVel = normalize(angVel);
//
//	angVel = angVel * (velocity.length() / radius);
//
//	SetAccel(vec3(0.2f,-0.2f,0.2f));
//	Physics::SimpleNewtonMotion(*this, deltaTime_);
//
//	if (model)
//	{
//		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
//		box.transform = model->GetTransform(modelInstance);
//	}
//}

void GameObject::MoveTest(const float deltaTime_)
{
	//SetAccel(vec3(0.2f,-0.35f,0.2f));
	//Physics::SimpleNewtonMotion(*this, deltaTime_);
	vec3 pos;
	
	vec3 targetPosition(8, 0, -5);
	vec3 diff = targetPosition - this->GetPosition();

	float distance = sqrtf(dot(diff, diff));

	if (distance > 0.1f)
	{
		vec3 moveDir = normalize(diff);
		pos = this->GetPosition() + moveDir * 2.0f * deltaTime_;
		this->SetPosition(this->GetPosition() + moveDir * 2.0f * deltaTime_);
	}
	else
	{
		this->SetPosition(targetPosition);
	}

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);
		setBoxTransform();
	}
}

void GameObject::Seek(vec3 destination_)
{
	//position += velocity * time;
	//orientation += rotation * time;
	//velocity += steering.linear * time;
	//rotation += steering.angular * time;
	//// clip velocity to a max speed, if needed 
	//// this might've been done in the algorithm 
	//if velocity.length() > maxSpeed  velocity.normalize velocity *= maxSpeed;

}


void GameObject::SetTag(string tag_)
{
	tag = tag_;
}

void GameObject::SetHit(bool hit_, int buttonType_)
{
	hit = hit_;

	if (hit)
	{
		if (GetComponent<AudioSource>() != nullptr)
		{
			GetComponent<AudioSource>()->OnCreate(this);
			GetComponent<AudioSource>()->playSound();
			cout << tag << " play sound" << endl;
		}

		cout << tag << " was hit" << endl;
	}
}

void GameObject::SetDelay(bool delay_)
{
	delayedUpdate = delay_;
}

void GameObject::SetAccel(vec3 accel_)
{
	accel = accel_;
}

void GameObject::SetAngVel(vec3 angVel_)
{
	angVel = angVel_;
}

void GameObject::SetQuaternion(quat q_)
{
	quaternion = q_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);
		setBoxTransform();
	}
}

void GameObject::DelayRender(const float deltaTime_)
{
	for (auto entry : delayComponent)
	{
		entry->Update(deltaTime_);
	}
}

void GameObject::setBoxTransform()
{

	if (model->getJointCount() > 0)
	{
		mat4 correctTransform;
		box.transform = model->GetTransform(modelInstance) * rotate(correctTransform, -90.0f, vec3(1, 0, 0));
	}
	else
	{
		box.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::increaseBB(vec3 rate)
{
	scale = rate;

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);

		box.transform = model->GetTransform(modelInstance);
		collidebox.transform = model->GetTransform(modelInstance);

		//setBoxTransform();
		box.minVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
		collidebox.minVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
		box.maxVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
		collidebox.maxVert *= scale.x > 1.0f ? scale : (scale / 2.0f);

	}
}

void GameObject::increaseBBZ(vec3 rate)
{
	scale = rate;

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale, quaternion);

		box.transform = model->GetTransform(modelInstance);
		collidebox.transform = model->GetTransform(modelInstance);

		//setBoxTransform();
		box.minVert *= scale.z > 1.0f ? scale : (scale / 2.0f);
		collidebox.minVert *= scale.z > 1.0f ? scale : (scale / 2.0f);
		box.maxVert *= scale.z > 1.0f ? scale : (scale / 2.0f);
		collidebox.maxVert *= scale.z > 1.0f ? scale : (scale / 2.0f);

	}
}


BoundingBox GameObject::GetBoundingBox() const
{
	return box;
}

BoundingBox GameObject::GetCollideBoundingBox() const
{
	return collidebox;
}
