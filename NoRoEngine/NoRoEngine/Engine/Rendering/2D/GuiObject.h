#ifndef GUIOBJECT_H
#define GUIOBJECT_H


#include "GuiComponents.h"
#include "../../Math/AI/SteeringOutput.h"
#include "../../Math/Quaternion.h"
#include "../../Math/Physics/Physics.h"


class GuiObject
{
public:
	GuiObject(vec2 position_);
	~GuiObject();

	void Draw(Camera* camera_);
	bool isInside(vec2 mousePosition_);
	string GetTag() const;
	void SetTag(string tag_);
	bool getHit() { return hit; }
	void setHit(bool hit_) { hit = hit_; }
	void Destroy()
	{
		for (size_t i = 0; i < GuiComponentContainer.size(); i++)
		{
			if (GuiComponentContainer[i])
			{
				delete GuiComponentContainer[i];
				GuiComponentContainer[i] = nullptr;
			}
		}
	}
	template<typename T, typename ... Args>
	void AddComponent(Args&& ... args_)
	{
		T* b2 = new T(std::forward<Args>(args_)...);

		if (GuiComponents* d = dynamic_cast<GuiComponents*>(b2))
		{
			if (!GetComponent<T>())
			{
				GuiComponentContainer.push_back(d);
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
		if (GuiComponentContainer.size() != 0)
		{
			for (auto i = 0; i < GuiComponentContainer.size(); i++)
			{
				if (T* d = dynamic_cast<T*>(GuiComponentContainer[i]))
				{
					return d;
				}
				else
				{
					return nullptr;
				}
			}
		}
	}

	template<typename T>
	void RemoveComponent()
	{

		for (size_t i = 0; i < GuiComponentContainer.size(); i++)
		{
			if (T* d = dynamic_cast<T*>(GuiComponentContainer[i]))
			{
				delete GuiComponentContainer[i];
				GuiComponentContainer[i] = nullptr;

				GuiComponentContainer.erase(GuiComponentContainer.begin() + i);
			}
		}
	}

private:

	bool hit;
	string tag;
	vec2 position;
	std::vector<GuiComponents*> GuiComponentContainer;

};
#endif // !GUIOBJECT_H



