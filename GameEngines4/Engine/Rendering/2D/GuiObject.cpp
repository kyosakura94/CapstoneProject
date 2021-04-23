#include "GuiObject.h"

GuiObject::GuiObject(vec2 position_)
{
	position = position_;
}

GuiObject::~GuiObject()
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

void GuiObject::Draw(Camera* camera_)
{
	
	if (GuiComponentContainer.size() > 0)
	{
		for (size_t i = 0; i < GuiComponentContainer.size(); i++)
		{
			GuiComponentContainer[i]->Draw(camera_, position);
		}
	}
}

bool GuiObject::isInside(vec2 mousePosition_)
{
	if (GuiComponentContainer.size() > 0)
	{
		for (size_t i = 0; i < GuiComponentContainer.size(); i++)
		{
			if (GuiComponentContainer[i]->FindContainingPoint(mousePosition_, position))
			{
				cout << "inside" << endl;
				return true;
			}
		}
	}
	
	return false;
}

string GuiObject::GetTag() const
{
	return tag;
}

void GuiObject::SetTag(string tag_)
{
	tag = tag_;
}
