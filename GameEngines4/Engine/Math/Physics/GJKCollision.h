#ifndef GJKCOLLISION_H
#define GJKCOLLISION_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Rendering/3D/GameObject.h"

using namespace std;
using namespace glm;



class GJKCollision
{
public:
	GJKCollision();
	~GJKCollision();

	bool CollisonCheck();
	bool CollisonCheck(GameObject * obj1, GameObject * obj2);
	bool myfn(int i, int j) { return i < j; }
	vec3 smallest1(vector<vec3> aray_, vec3 direct_);
	vec3 smallest(vector<vec3> aray_, vec3 direct_);
	vec3 Center(vector<vec3> aray_);
	vec3 largest(vector<vec3> array_, vec3 direct_);

	bool check(vec3 PointA, vec3 PointB, vec3 PointC, vector<vec3> shapeOne, vector<vec3> shapeTwo);
private:
	vec3 p1;
};

#endif // !GJKCOLLISION_H

