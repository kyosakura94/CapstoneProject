#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Component
{
public:
	inline Component() : parent(nullptr) {}
	virtual ~Component() { parent = nullptr; }

	virtual bool OnCreate(GameObject * parent) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual bool delayedUpdate() = 0;

protected:

	GameObject* parent;
};
#endif

