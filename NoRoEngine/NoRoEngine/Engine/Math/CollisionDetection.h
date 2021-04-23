#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "../Camera/Camera.h"

struct Ray;
struct BoundingBox;

class CollisionDetection
{
public:
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator = (const CollisionDetection&) = delete;
	CollisionDetection& operator = (CollisionDetection&&) = delete;

	CollisionDetection() = delete;
	~CollisionDetection();

	static Ray ScreenPosToWorldRay(vec2 mouseCoords_, vec2 screenSize_, Camera * camera_);
	static bool RayObbIntersection(Ray* ray_, BoundingBox * box_);

private:
	
	
};
#endif // !COLLISIONDETECTION_H


