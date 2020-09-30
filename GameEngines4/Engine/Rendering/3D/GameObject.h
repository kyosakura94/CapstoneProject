#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
using namespace glm;
using namespace std;
class GameObject
{
public:
	GameObject(Model *model_, glm::vec3 position_ = vec3());
	~GameObject();
	void Render(Camera *camera_);
	void Update(const float deltaTime_);

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	bool GetHit() const;
	string GetTag() const;
	float GetAngle() const;

	void SetPosition(glm::vec3 position_);
	void SetRotation(glm::vec3 rotation_);
	void SetScale(glm::vec3 scale_);
	void SetAngle(float angle_);
	void SetTag(string tag_);
	void SetHit(bool hit_, int buttonType_);

	BoundingBox GetBoundingBox() const;

	
	
private:
	Model *model;
	int modelInstance;

	glm::vec3 position;
	float angle;
	glm::vec3 rotation;
	glm::vec3 scale;
	BoundingBox box;
	string tag;

	bool hit;

};

#endif // !1