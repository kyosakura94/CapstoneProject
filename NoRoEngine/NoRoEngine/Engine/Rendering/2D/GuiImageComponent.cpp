#include "GuiImageComponent.h"

GuiImageComponent::GuiImageComponent() : angle_(0), scale_(1), offset(vec2(0, 0)), tintColor_(vec4(1.0f, 1.0f, 1.0f, 1.0f))
{

}

bool GuiImageComponent::OnCreate(string nameImage_)
{
	GLuint shaderProgram_ = ShaderHandler::getInstance()->GetShader("spriteShader");
	surface = new SpriteSurface(nameImage_, scale_, angle_, tintColor_, shaderProgram_);

	return true;
}

void GuiImageComponent::Update(float deltaTime)
{
}

void GuiImageComponent::Draw(Camera* camera_, vec2 parentPosition_)
{
	surface->Draw(camera_, parentPosition_);
}

bool GuiImageComponent::FindContainingPoint(vec2 mousePosition_, vec2 guiPosition_)
{
	float height = surface->getHeight();
	float width = surface->getWidth();

	vec2 AB(height, 0);
	vec2 AM(mousePosition_.x - guiPosition_.x + (height/2), mousePosition_.y - guiPosition_.y - (width/2));
	vec2 AD(0, -width);

	//(0 < AM⋅AB < AB⋅AB)∧(0 < AM⋅AD < AD⋅AD)
	if (0 < dot(AM, AB) && dot(AM, AB) < dot(AB, AB)
		&& 0 < dot(AM, AD) && dot(AM, AD) < dot(AD, AD))
	{
		return true;
	}
	else
	{
		return false;
	}
}

