#include "GuiImageComponent.h"

GuiImageComponent::GuiImageComponent(): angle_(0), scale_(1), offset(vec2(0,0)), tintColor_(vec4(1.0f, 1.0f, 1.0f, 1.0f))
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
	bool result = false;
	if (surface != nullptr)
	{
		float tMin = surface->getWidth();
		float tMax = surface->getHeight();


		vec2 delta = guiPosition_ - mousePosition_;

		{
			float e = dot(guiPosition_, delta);
			float f = dot(vec2(0.0f), guiPosition_);

			if (fabs(f) > 0.001f)
			{
				float t1 = (e + tMin) / f;
				float t2 = (e + tMax) / f;

				if (t1 > t2)
				{
					float w = t1;
					t1 = t2;
					t2 = w;
				}
				if (t2 < tMax)
				{
					tMax = t2;
				}
				if (t1 > tMin)
				{
					tMin = t1;
				}

				if (tMax < tMin)
				{
					return false;
				}
			}
			else
			{
				if (-e + tMin > 0.0f || -e + tMax < 0.0f)
				{
					return false;
				}
			}
		}
	}
	return result;
}
