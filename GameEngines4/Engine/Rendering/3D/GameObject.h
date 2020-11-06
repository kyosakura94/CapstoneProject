#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
#include "Component.h"
#include "../../Math/AI/SteeringOutput.h"
#include "../../Math/Quaternion.h"
#include "../../Math/Physics/Physics.h"
#include "../../FX/Audio/AudioSource.h"

using namespace glm;
using namespace std;
class GameObject
{
public:
	GameObject(Model *model_, glm::vec3 position_ = vec3());
	~GameObject();
	void Render(Camera *camera_);
	void Update(const float deltaTime_);
	void Update(SteeringOutput steering, const float deltaTime_);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	glm::vec3 GetVelocity();
	bool GetHit() const;
	string GetTag() const;
	float GetAngle() const;
	vec3 GetAccel();
	vec3 GetAngVel();
	Quaternion getQuaternion();

	void SetPosition(glm::vec3 position_);
	void SetRotation(glm::vec3 rotation_);
	void SetScale(glm::vec3 scale_);
	void SetVelocity(glm::vec3 Velocity_);
	void SetAngle(float angle_);
	void Move(SteeringOutput steering, const float deltaTime_);
	void MoveTest(const float deltaTime_);
	void Seek(vec3 destination_);
	void SetTag(string tag_);
	void SetHit(bool hit_, int buttonType_);
	void SetAccel(vec3 accel_);
	void SetAngVel(vec3 angVel_);
	void SetQuaternion(Quaternion q_);

	BoundingBox GetBoundingBox() const;

	template<typename T, typename ... Args>
	void AddComponent(Args&& ... args_)
	{
		T* b2 = new T(std::forward<Args>(args_)...);

		if (dynamic_cast<Component*>(b2))
		{
			if (!GetComponent<T>())
			{
				componentContainer.push_back(b2);
				b2->OnCreate(this);
			}
			else
			{
				cout << " Have the same component" << endl;
				delete b2;
				b2 = nullptr;
				return;
			}
		}
		else
		{
			cout << " Not child of object" << endl;
			delete b2;
			b2 = nullptr;
		}
	}
	
	template<typename T>
	T* GetComponent() 
	{
		if (componentContainer.size() != 0)
		{
			for (auto i = 0; i < componentContainer.size(); i++)
			{
				if (T* d = dynamic_cast<T*>(componentContainer[i]))
				{
					return d;
				}
			}
		}

		return nullptr;
	}
	
	template<typename T>
	void RemoveComponent() 
	{
		for (size_t i = 0; i < componentContainer.size(); i++)
		{
			if (T* d = dynamic_cast<T*>(componentContainer[i]))
			{
				delete componentContainer[i];
				componentContainer[i] = nullptr;
				componentContainer.erase(componentContainer.begin() + i);
				break;
			}
		}
	}
	
private:
	Model *model;
	int modelInstance;

	glm::vec3 position;
	float angle;
	float targetSpeed;
	float radius;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 velocity;
	glm::vec3 orientation;
	glm::vec3 accel;

	Quaternion q;
	vec3 angVel;
	BoundingBox box;
	string tag;


	std::vector<Component*> componentContainer;
	bool hit;
	//static_cast<float>
};

#endif // !1


