#ifndef GUIIMAGECOMPONENT_H
#define GUIIMAGECOMPONENT_H

#include "GuiComponents.h"
#include "../../Camera/Camera.h"
#include "../../Core/CoreEngine.h"

class GuiImageComponent : public GuiComponents
{
public:
	GuiImageComponent();
	virtual ~GuiImageComponent() { surface = nullptr; }

	virtual bool OnCreate(string nameImage_);
	virtual void Update(float deltaTime);
	virtual void Draw(Camera * camera_, vec2 parentPosition_);
	virtual bool FindContainingPoint(vec2 mousePosition_, vec2 guiPosition_);

private:
	SpriteSurface * surface;
	vec2 offset;
	float scale_, angle_;
	vec4 tintColor_;

};
#endif // !GUIIMAGECOMPONENT_H



