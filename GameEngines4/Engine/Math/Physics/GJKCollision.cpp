#include "GJKCollision.h"
#include <algorithm>

GJKCollision::~GJKCollision()
{

}

bool GJKCollision::CollisonCheck()
{
	vec3 orgin(0,0,0);

	vec3 A(0.5f, 0.8f, 0.3f);
	vec3 B(0,0,0);
	vec3 C(0.5f, 0, 0.9f);
	vec3 D(1, 0, 0);

	vec3 E(0.5f, 0.1f, 0.3f);
	vec3 F(0, -0.7f, 0);
	vec3 G(0.5f, -0.7f, 0.9);
	vec3 H(1, -0.7, 0);


	vec3 c1 = (A + B + C + D) / 4.0f;
	vec3 c2 = (E + F + G + H) / 4.0f;

	vec3 d1 = vec3(round(c2 - c1).x, round(c2 - c1).y, round(c2 - c1).z);

	vec3 d2 = vec3(-(d1.x), -(d1.y), -(d1.z));

	vec3 shapeOne[] = { A, B, C, D};
	vec3 shapeTwo[] = { E, F, G, H};

	vec3 PointA = largest(shapeOne, d1) - smallest(shapeTwo, d1);
	vec3 PointB = largest(shapeOne, d2) - smallest1(shapeTwo, d2);

	vec3 ab = PointB - PointA;
	vec3 ao = vec3(0, 0, 0) - PointA;

	vec3 d = cross((cross(ab, ao)), ab);

	vec3 PointC = largest(shapeOne, d) - smallest(shapeTwo, d);

	return check(PointA, PointB, PointC, shapeOne, shapeTwo);
}

vec3 GJKCollision::smallest(vec3 aray_[], vec3 direct_)
{
	float maxValue = 999;

	vec3 result = aray_[0];

	for (size_t i = 0; i <= aray_->length(); i++)
	{
		vec3 check = aray_[i];
		float tmp = dot(check, direct_);
		if (tmp <= maxValue)
		{
			maxValue = tmp;
			result = aray_[i];
		}
	}

	return result;
}

vec3 GJKCollision::smallest1(vec3 aray_[], vec3 direct_)
{
	float maxValue = 999;

	vec3 result = aray_[0];

	for (size_t i = 0; i <= aray_->length(); i++)
	{
		vec3 check = aray_[i];
		float tmp = dot(check, direct_);
		if (tmp < maxValue)
		{
			maxValue = tmp;
			result = aray_[i];
		}
	}

	return result;
}

vec3 GJKCollision::largest(vec3 aray_[], vec3 direct_)
{
	float minValue = -999;

	vec3 result = aray_[0];

	for (size_t i = 0; i <= aray_->length(); i++)
	{
		vec3 check = aray_[i];
		float tmp = dot(aray_[i], direct_);
		if (tmp > minValue)
		{
			minValue = tmp;
			result = aray_[i];
		}
	}

	return result;
}

bool GJKCollision::check(vec3 PointA, vec3 PointB, vec3 PointC, vec3 shapeOne[], vec3 shapeTwo[])
{
	vec3 ab = PointB - PointA;
	vec3 ac = PointC - PointA;

	vec3 N = cross(ab, ac);
	vec3 N1 = N;
	vec3 N2 = -N;
	vec3 T = (PointA + PointB + PointC) / 3.0f;
	vec3 T0 = vec3(0, 0, 0) - T;

	vec3 d;
	if (dot(N1, T0) > 0)
	{
		d = N1;
	}
	else
	{
		d = N2;
	}

	vec3 PointD = largest(shapeOne, d) - smallest1(shapeTwo, d);

	float abcd[] = { PointA.x , PointA.y, PointA.z, 1,
				   PointB.x , PointB.y, PointB.z, 1,
				   PointC.x , PointC.y, PointC.z, 1,
				   PointD.x , PointD.y, PointD.z, 1 };

	float obcd[] = { 0 , 0, 0, 1,
				   PointB.x , PointB.y, PointB.z, 1,
				   PointC.x , PointC.y, PointC.z, 1,
				   PointD.x , PointD.y, PointD.z, 1 };

	float a0cd[] = { PointA.x , PointA.y, PointA.z, 1,
				   0 , 0, 0, 1,
				   PointC.x , PointC.y, PointC.z, 1,
				   PointD.x , PointD.y, PointD.z, 1 };

	float ab0d[] = { PointA.x , PointA.y, PointA.z, 1,
				   PointB.x , PointB.y, PointB.z, 1,
				   0 , 0, 0, 1,
				   PointD.x , PointD.y, PointD.z, 1 };

	float abc0[] = { PointA.x , PointA.y, PointA.z, 1,
				   PointB.x , PointB.y, PointB.z, 1,
				   PointC.x , PointC.y, PointC.z, 1,
				   0 , 0, 0, 1 };


	mat4 nar1 = make_mat4(abcd);
	mat4 nar2 = make_mat4(obcd);
	mat4 nar3 = make_mat4(a0cd);
	mat4 nar4 = make_mat4(ab0d);
	mat4 nar5 = make_mat4(abc0);

	float D0 = determinant(nar1);
	float D1 = determinant(nar2);
	float D2 = determinant(nar3);
	float D3 = determinant(nar4);
	float D4 = determinant(nar5);


	if (D0 < 0)
	{
		vector<vec3> myVector;
		if (D1 < 0 && D2 < 0 && D3 < 0 && D4 < 0)
		{
			return true;
		}

		if (D1 < 0)
		{
			myVector.push_back(PointA);
		}
		if (D2 < 0)
		{
			myVector.push_back(PointB);
		}

		if (D3 < 0)
		{
			myVector.push_back(PointC);
		}

		if (D4 < 0)
		{
			myVector.push_back(PointD);
		}

		if (myVector.size() >= 3)
		{
			return check(myVector[0], myVector[1], myVector[2], shapeOne, shapeTwo);
		}
	}

	if (D0 > 0)
	{
		vector<vec3> myVector;
		if (D1 > 0 && D2 > 0 && D3 > 0 && D4 > 0)
		{
			return true;
		}

		if (D1 > 0)
		{
			myVector.push_back(PointA);
		}
		if (D2 > 0)
		{
			myVector.push_back(PointB);
		}

		if (D3 > 0)
		{
			myVector.push_back(PointC);
		}

		if (D4 > 0)
		{
			myVector.push_back(PointD);
		}

		if (myVector.size() >= 3)
		{
			return check(myVector[0], myVector[1], myVector[2], shapeOne, shapeTwo);
		}
	}

	return false;
}

GJKCollision::GJKCollision()
{

}
