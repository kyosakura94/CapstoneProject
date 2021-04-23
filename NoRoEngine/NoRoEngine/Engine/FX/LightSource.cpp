#include "LightSource.h"

LightSource::LightSource(vec3 position_, vec3 lightColor_, float ambientvalue_, float diffuseValue_): position(0,0,0), lightColor(1,0,0), ambientvalue(1.0f), diffuseValue(1.0f)
{
	position = position_;
	lightColor = lightColor_;
	ambientvalue = ambientvalue_;
	diffuseValue = diffuseValue_;
}

vec3 LightSource::GetLightPosition()
{
	return position;
}

vec3 LightSource::GetLightColor()
{
	return lightColor;
}

float LightSource::GetAmbientValue()
{
	return ambientvalue;
}

float LightSource::GetDiffuseValue()
{
	return diffuseValue;
}

void LightSource::SetLightPosition(vec3 position_)
{
	position = position_;
}

void LightSource::SetLightColor(vec3 color_)
{
	lightColor = color_;
}

void LightSource::SetAmbientValue(float ambientvalue_)
{
	ambientvalue = ambientvalue_;
}

void LightSource::SetDiffuseValue(float diffusevalue_)
{
	diffuseValue = diffusevalue_;
}


LightSource::~LightSource()
{
}
