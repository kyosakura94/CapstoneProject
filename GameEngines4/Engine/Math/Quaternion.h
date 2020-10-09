#ifndef QUATERNION_H
#define QUATERNION_H

#include <glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "../../Engine/Math/EulerAngles.h"

using namespace glm;
using namespace std;
class Quaternion
{
private:

	vec4 quaternion;

public:

	Quaternion();
	Quaternion(vec3 axis, float angleInDegrees);

	inline const Quaternion operator * (const Quaternion& q) const 
	{
		
		vec3 v2(q.quaternion.x, q.quaternion.y, q.quaternion.z);

		vec3 v1(quaternion.x, quaternion.y, quaternion.z);

		float w1 = quaternion.w;
		float w2 = q.quaternion.w;

		vec3 ijk(v2 * w1 + v1 * w2 + cross(v1, v2));
		float dotproduct = dot(v1, v2);
		Quaternion result(w1*w2 - dotproduct, ijk.x, ijk.y, ijk.z);

		return result;

	}

	inline const Quaternion operator += (const Quaternion& q) const 
	{
		Quaternion result(quaternion.w + q.quaternion.w, quaternion.x + q.quaternion.x, quaternion.y + q.quaternion.y, quaternion.z + q.quaternion.z);
		return result;
	}
	
	inline const Quaternion operator * (const float q) const 
	{
		Quaternion result(q * quaternion.w, quaternion.x + q, quaternion.y + q, quaternion.z + q);
		return result;
	}

	inline void set(float w_, float x_, float y_, float z_) {
		quaternion.w = w_; quaternion.x = x_; quaternion.y = y_; quaternion.z = z_;
	}
	inline Quaternion(float w_, float x_, float y_, float z_) {
		set(w_, x_, y_, z_);
	}
	inline const Quaternion conjugate() const {
		return Quaternion(quaternion.w, -quaternion.x, -quaternion.y, -quaternion.z);
	}

	float Mag();
	Quaternion Normalize();
	void Inverse();
	vec3 Rotate(vec3 vec);

	mat3 ConvertToMatrix(const Quaternion &q);
	EulerAngles ConvertEuler();

	inline void print() {
		printf("%1.8f %1.8f %1.8f %1.8f\n", quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}
	~Quaternion();

};




#endif