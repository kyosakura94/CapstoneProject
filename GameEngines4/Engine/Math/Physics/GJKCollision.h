#ifndef GJKCOLLISION_H
#define GJKCOLLISION_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;



class GJKCollision
{
public:
	GJKCollision();
	~GJKCollision();

	bool CollisonCheck();
	bool myfn(int i, int j) { return i < j; }
	vec3 smallest1(vec3 aray_[], vec3 direct_);
	vec3 smallest(vec3 aray_[], vec3 direct_);
	vec3 largest(vec3 aray_[], vec3 direct_);

	bool check(vec3 PointA, vec3 PointB, vec3 PointC, vec3 shapeOne[], vec3 shapeTwo[]);
private:
	vec3 p1;
};

#endif // !GJKCOLLISION_H

