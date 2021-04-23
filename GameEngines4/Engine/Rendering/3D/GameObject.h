#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
#include "Component.h"
#include "../../Math/AI/SteeringOutput.h"
#include "../../Math/Quaternion.h"
#include "../../Math/Physics/Physics.h"
#include "../../FX/Audio/AudioSource.h"
#include "../../Animation/animatedModel/AnimatedModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;
using namespace std;
class GameObject
{
public:
	GameObject(AnimatedModel* model_, glm::vec3 position_ = vec3(), glm::quat quaternion_ = quat());
	GameObject(Model* model_, glm::vec3 position_ = vec3(), glm::quat quaternion_ = quat());
	GameObject(Model* model_, glm::vec3 position_ = vec3());
	~GameObject();
	void Destroy();
	void Render(Camera* camera_);
	void Update(const float deltaTime_);
	void Update(SteeringOutput steering, const float deltaTime_);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	glm::vec3 GetVelocity();
	bool GetHit() const;
	bool GetDelayUpdate();
	string GetTag() const;
	float GetAngle() const;
	vec3 GetAccel();
	vec3 GetAngVel();
	quat getQuaternion();
	int getModelIntances() { return modelInstance; };
	void SetPosition(glm::vec3 position_);
	void SetLifeTime(float life_);
	void SetRotation(glm::vec3 rotation_);
	void SetScale(glm::vec3 scale_);
	void SetVelocity(glm::vec3 Velocity_);
	void SetAngle(float angle_);
	void Move(SteeringOutput steering, const float deltaTime_);
	void MoveTest(const float deltaTime_);
	void Seek(vec3 destination_);
	void SetTag(string tag_);
	void SetHit(bool hit_, int buttonType_);
	void SetDelay(bool delay_);
	void SetAccel(vec3 accel_);
	void SetAngVel(vec3 angVel_);
	void SetQuaternion(quat q_);
	void DelayRender(const float deltaTime_);
	void setBoxTransform();
	void increaseBB(vec3 rate);
	void increaseBBZ(vec3 rate);
	BoundingBox GetBoundingBox() const;
	BoundingBox GetCollideBoundingBox() const;

	Model* getModel() { return model; }
	template<typename T, typename ... Args>
	void AddComponent(Args&& ... args_)
	{
		T* b2 = new T(std::forward<Args>(args_)...);

		if (dynamic_cast<Component*>(b2))
		{
			if (!GetComponent<T>())
			{
				
				if (b2->delayedUpdate() == true)
				{
					delayedUpdate = true;
					delayComponent.push_back(b2);
				}
				else
				{
					componentContainer.push_back(b2);
				}

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
		if (delayComponent.size() != 0)
		{
			for (auto i = 0; i < delayComponent.size(); i++)
			{
				if (T* d = dynamic_cast<T*>(delayComponent[i]))
				{
					return d;
				}
			}

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
		}
		else
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

	float getLifetime() { return lifeTime; }
	
private:
	

	Model *model;
	int modelInstance;
	float lifeTime;
	glm::vec3 position;
	float angle;
	float targetSpeed;
	float radius;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 velocity;
	glm::vec3 orientation;
	glm::vec3 accel;
	glm::quat quaternion;
	quat q;
	vec3 angVel;
	BoundingBox box;
	BoundingBox collidebox;
	string tag;


	std::vector<Component*> componentContainer;
	std::vector<Component*> delayComponent;
	bool hit;

	//only for particle emmtter objects
	bool delayedUpdate;

	//static_cast<float>
};

#endif // !1


