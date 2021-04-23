#ifndef GUICOMPONENTS_H
#define GUICOMPONENTS_H

#include "SpriteSurface.h"
#include "../../Camera/Camera.h"

class GuiComponents
{
public:
	inline GuiComponents() {}
	virtual ~GuiComponents() { }

	virtual bool OnCreate(string nameImage_) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(Camera* camera_, vec2 parentPosition_) = 0;
	virtual bool FindContainingPoint(vec2 mousePosition_, vec2 guiPosition_) = 0;

private:
};



#endif // !GUICOMPONENTS_H

