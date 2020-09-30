#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>
struct Plane
{
	float x, y, z, d;
	
	inline void set(float x_, float y_, float z_, float d_) 
	{
		x = x_; y = y_; z = z_, d = d_;
	}

	explicit Plane(float s = 0.0f) {
		set(s, s, s, s);
	}
	Plane(float x, float y, float z, float d)
	{
		set(x, y, z, d);
	}

	Plane(const Plane& p) {
		set(p.x, p.y, p.z, p.d);
	}

	void normalize() {
		float a = sqrt(x * x + y * y + z * z);
		x /= a;
		y /= a;
		z /= a;
		d /= a;
	}
	float distance(vec3 &p)
	{
		return (d + (x * p.x + y * p.y + z * p.z));
	}

};
#endif // !PLANE_H

