#include "Quaternion.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS (M_PI / 180.0f)
#endif 
Quaternion::Quaternion()
{
	quaternion.x = 0.0f;
	quaternion.y = 0.0f;
	quaternion.z = 0.0f;
	quaternion.w = 1.0f;
}

Quaternion::Quaternion(vec3 axis, float angleInDegrees)
{
	angleInDegrees *= DEGREES_TO_RADIANS; 
	set(cos(angleInDegrees / 2.0f), 
		sin(angleInDegrees / 2.0f) * axis.x, 
		sin(angleInDegrees / 2.0f) * axis.y, 
		sin(angleInDegrees / 2.0f) * axis.z);
}


float Quaternion::Mag()
{
	float mag = quaternion.length();
	return mag;
}

Quaternion Quaternion::Normalize()
{ 
	float m = Mag();
	quaternion.x /= m;
	quaternion.y /= m;
	quaternion.z /= m;
	quaternion.w /= m;

	return Quaternion(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
}

void Quaternion::Inverse()
{
	float m = Mag();
	conjugate();
	quaternion.x /= m;
	quaternion.y /= m;
	quaternion.z /= m;
	quaternion.w /= m;
}

vec3 Quaternion::Rotate(vec3 vec)
{
	vec3 result;
	vec3 u(quaternion.x, quaternion.y, quaternion.z);

	// Extract the scalar part of the quaternion
	float s = quaternion.w;

	// Do the math
	result = u * 2.0f * dot(u, vec) +  vec * (s * s - dot(u, u)) + cross(u,vec)  * s * 2.0f;

	return result;
}

mat3 Quaternion::ConvertToMatrix(const Quaternion &q)
{
	vec3 v(q.quaternion.x, q.quaternion.y, q.quaternion.z);
	float w = q.quaternion.w;
	return mat3(
		(1.0f - 2.0f * v.y * v.y - 2.0f * v.z * v.z), 
		(2.0f*v.x*v.y + 2.0f * v.z*w), 
		(2.0f*v.x*v.z - 2.0f*v.y*w),
		(2.0f*v.x*v.y - 2.0f*v.z*w), 
		(1.0f - 2.0f*v.x*v.x - 2.0f*v.z*v.z), 
		(2.0f * v.y*v.z + 2.0f * v.x*w),
		(2.0f*v.x*v.z + 2.0f*v.y*w), 
		(2.0f*v.y*v.z - 2 * v.x*w), 
		(1.0f - 2.0f*v.x*v.x - 2.0f*v.y*v.y));
}

EulerAngles Quaternion::ConvertEuler()
{
	EulerAngles result;

	// roll (x-axis rotation)
	float sinr_cosp = +2.0 * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	float cosr_cosp = +1.0 - 2.0 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);

	result.xAxis = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = +2.0 * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
	if (fabs(sinp) >= 1)
		result.yAxis = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		result.yAxis = asin(sinp);

	// yaw (z-axis rotation)
	float siny_cosp = +2.0 * (quaternion.w  * quaternion.z + quaternion.x * quaternion.y);
	float cosy_cosp = +1.0 - 2.0 * (quaternion.y  * quaternion.y + quaternion.z * quaternion.z);
	result.zAxis = atan2(siny_cosp, cosy_cosp);

	return result;
}

Quaternion::~Quaternion()
{
}


