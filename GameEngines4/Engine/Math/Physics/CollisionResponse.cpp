#include "CollisionResponse.h"

CollisionResponse::CollisionResponse()
{
	float m_ship = 100.0f;
	float m_rock = 1;

	vec3 v_ship = vec3(0,0,0);
	vec3 v_rock = vec3(20000,10000,20000);

	vec3 w_ship = vec3(0,0,0);

	vec3 r_ship = vec3(8, -1, 0);
	vec3 r_rock = vec3(-2, 1, 0);

	mat3 I_ship = mat3(20, 0, 0, 0, 40, 0, 0, 0, 20);
	mat3 I_rock = mat3(0.1f, 0, 0, 0, 0.1f, 0, 0, 0, 0.1f);

	float e = 0.5f;

	vec3 n = vec3(0.6666f,  0.3333f , 0.6666f);

	vec3 vr = v_rock - v_ship;

	
	float k = vr.x * vr.x + vr.y * vr.y + vr.z * vr.z;

	float s = std::sqrt(k);
	float top = s * (e + 1);


	mat3 I1_Rock = inverse(I_rock);
	mat3 I2_ship = inverse(I_ship);

	float d1 = 1 / m_rock;
	float d2 = 1 / m_ship;

	float d3 = dot((I1_Rock * cross(cross(r_rock,n), r_rock)), n);
	float d4 = dot((I2_ship * cross(cross(r_ship,n), r_ship)), n);

	float j = top / (d1 + d2 + d3 + d4);


	vec3 v2_ship = v_ship + ((-j * n) / m_ship);
	vec3 w2_ship = w_ship + (I2_ship * cross(r_ship, (-j * n)));

	cout << "J : " << to_string(j) << endl;
	cout << "Linear Velocity : " << to_string(v2_ship) << endl;
	cout << "Angular Impulse : " << to_string(w2_ship) << endl;


}

CollisionResponse::~CollisionResponse()
{
}
