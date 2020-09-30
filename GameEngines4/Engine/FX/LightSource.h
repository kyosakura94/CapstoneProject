#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <glm/vec3.hpp>

using namespace glm;
class LightSource
{
public:
	LightSource(vec3 position_, vec3 lightColor_, float ambientvalue_, float diffuseValue_);
	vec3 GetLightPosition();
	vec3 GetLightColor();
	float GetAmbientValue();
	float GetDiffuseValue();

	void SetLightPosition(vec3 position_);
	void SetLightColor(vec3 color_);
	void SetAmbientValue(float ambientvalue_);
	void SetDiffuseValue(float diffusevalue_);

	~LightSource();
private:
	vec3 position;
	vec3 lightColor;
	float ambientvalue;
	float diffuseValue;
	
};
#endif // !LIGHTSOURCE


