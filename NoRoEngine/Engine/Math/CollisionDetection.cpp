#include "CollisionDetection.h"

#include "Ray.h"
#include "../Core/CoreEngine.h"


CollisionDetection::~CollisionDetection()
{
}

Ray CollisionDetection::ScreenPosToWorldRay(vec2 mouseCoords_, vec2 screenSize_, Camera * camera_)
{
	vec4 rayStart_NDC((mouseCoords_.x / screenSize_.x - 0.5f) * 2.0f, (mouseCoords_.y / screenSize_.y - 0.5f) * 2.0f, -1.0f, 1.0f);
	vec4 rayEnd_NDC((mouseCoords_.x / screenSize_.x - 0.5f) * 2.0f, (mouseCoords_.y / screenSize_.y - 0.5f) * 2.0f, 0.0f, 1.0f);

	mat4 inverse = glm::inverse(camera_->GetPerspective() * camera_->GetView());

	vec4 rayStart_World = inverse * rayStart_NDC;
	rayStart_World /= rayStart_World.w;

	vec4 rayEnd_World = inverse * rayEnd_NDC;
	rayEnd_World /= rayEnd_World.w;

	vec3 rayDir_World(rayEnd_World - rayStart_World);

	rayDir_World = normalize(rayDir_World);

	return Ray(vec3(rayStart_World), rayDir_World);
}

bool CollisionDetection::RayObbIntersection(Ray * ray_, BoundingBox * box_)
{
	double tMin = CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().x;
	double tMax = CoreEngine::GetInstance()->GetCamera()->GetClippingPlanes().y;

	vec3 obbPosition_World(box_->transform[3].x, box_->transform[3].y, box_->transform[3].z);

	vec3 delta = obbPosition_World - ray_->orgin;

	{
	vec3 xAxis(box_->transform[0].x, box_->transform[0].y, box_->transform[0].z);

	double e = dot(xAxis, delta);
	double f = dot(ray_->direction, xAxis);

	if (fabs(f) > 0.000000001f)
	{
		double t1 = (e + box_->minVert.x) / f;
		double t2 = (e + box_->maxVert.x) / f;

		if (t1 > t2)
		{
			double w = t1;
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
		if (-e + box_->minVert.x > 0.0f || -e + box_->maxVert.x < 0.0f)
		{
			return false;
		}
	}
	}
	//Y axis
	{
		vec3 yAxis(box_->transform[1].x, box_->transform[1].y, box_->transform[1].z);
		double e = dot(yAxis, delta);
		double f = dot(ray_->direction, yAxis);

		if (fabs(f) > 0.000000001f)
		{
			double t1 = (e + box_->minVert.y) / f;
			double t2 = (e + box_->maxVert.y) / f;

			if (t1 > t2)
			{
				double w = t1;
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
			if (-e + box_->minVert.y > 0.0f || -e + box_->maxVert.y < 0.0f)
			{
				return false;
			}
		}
	}
	//////Z axis
	{
		vec3 zAxis(box_->transform[2].x, box_->transform[2].y, box_->transform[2].z);
		double e = dot(zAxis, delta);
		double f = dot(ray_->direction, zAxis);

		if (fabs(f) > 0.000000001f)
		{
			double t1 = (e + box_->minVert.z) / f;
			double t2 = (e + box_->maxVert.z) / f;

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
			if (-e + box_->minVert.z > 0.0f || -e + box_->maxVert.z < 0.0f)
			{
				return false;
			}
		}
	}

	ray_->intersectionDist = tMin;

	return true;
}
