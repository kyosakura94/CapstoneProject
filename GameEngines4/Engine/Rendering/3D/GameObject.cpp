#include "GameObject.h"

GameObject::GameObject(Model *model_, vec3 position_) : model(nullptr)
{
	model = model_;
	position = position_;
	rotation = vec3(0.0f, 1.0f, 0.0f);
	scale = vec3(1.0f);
	angle = 0.0f;
	hit = false;
	if (model)
	{
		modelInstance = model->CreateInstance(position, angle, rotation, scale);
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

void GameObject::Render(Camera *camera_)
{
	if (model)
	{
		model->Render(camera_, tag);
	}
}

void GameObject::Update(const float deltaTime_)
{
	for (size_t i = 0; i < componentContainer.size(); i++)
	{
		componentContainer[i]->Update(deltaTime_);
	}
	//Move(position + vec3(0,0,0.005f));
	SetAngle(angle + 0.005f);
}

void GameObject::Update(SteeringOutput steering, const float deltaTime_)
{
	Move(steering, deltaTime_);
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

bool GameObject::GetHit() const
{
	return hit;
}

string GameObject::GetTag() const
{
	return tag;
}

float GameObject::GetAngle() const
{
	return angle;
}

void GameObject::SetPosition(glm::vec3 position_)
{
	position = position_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::SetRotation(glm::vec3 rotation_)
{
	rotation = rotation_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::SetScale(glm::vec3 scale_)
{
	scale = scale_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
		box.minVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
		box.maxVert *= scale.x > 1.0f ? scale : (scale / 2.0f);
	}
}

void GameObject::SetAngle(float angle_)
{
	angle = angle_;
	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
	}
}

void GameObject::Move(SteeringOutput steering, const float deltaTime_)
{
	float maxSpeed = 5.0;
	position += velocity * deltaTime_;
	orientation += rotation * deltaTime_;


	velocity += steering.getLinear() * deltaTime_;
	rotation += steering.getAngular() * deltaTime_;

	if (velocity.length() > maxSpeed) 
	{
		velocity = normalize(velocity);
		velocity *= maxSpeed;
	}
	

	if (model)
	{
		model->UpdateInstance(modelInstance, position, angle, rotation, scale);
		box.transform = model->GetTransform(modelInstance);
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
		cout << tag << " was hit" << endl;
	}
}

BoundingBox GameObject::GetBoundingBox() const
{
	return box;
}

//template<typename T>
//void GameObject::AddComponent()
//{
//	T* b2;
//
//	if (Component* d = dynamic_cast<Component*>(b2))
//	{
//		if (GetComponent())
//		{
//			componentContainer.push_back(d);
//			d->OnCreate(this);
//		}
//		else
//		{
//			cout << " Have the same component" << endl;
//			delete b2;
//			b2 = nullptr;
//			return;
//		}
//	}
//	else
//	{
//		cout << " Not child of object" << endl;
//		delete b2;
//		b2 = nullptr;
//	}
//}

//template<typename T>
//T* GameObject::GetComponent()
//{
//	if (componentContainer.size() != 0)
//	{
//		for (auto i = 0; i < componentContainer.size(); i++)
//		{
//			if (T* d = dynamic_cast<T*>(componentContainer[i]))
//			{
//				return d;
//			}
//			else
//			{
//				return nullptr;
//			}
//		}
//	}
//}

//template<typename T>
//void GameObject::RemoveComponent()
//{
//	for (size_t i = 0; i < componentContainer.size(); i++)
//	{
//		if (T* d = dynamic_cast<T*>(componentContainer[i]))
//		{
//			delete componentContainer[i];
//			componentContainer[i] = nullptr;
//
//			componentContainer.begin();
//			componentContainer.pop_back();
//		}
//	}
//}
