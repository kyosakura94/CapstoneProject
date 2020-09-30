#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <glm/glm.hpp>

using namespace glm;
using namespace std;
struct BoundingBox 
{
public:
	vec3 minVert, maxVert;
	mat4 transform;

	inline BoundingBox() {
		minVert = vec3();
		maxVert = vec3();
		transform = mat4();
	}

	inline BoundingBox(vec3 minVert_, vec3 maxVert_, mat4 transform_ ) 
	{
		minVert = minVert_;
		maxVert = maxVert_;
		transform = transform_;
	}
	inline bool Intersect(BoundingBox * box_) 
	{
		vec3 minCorner = GetTransfromedPoint(minVert, transform);
		vec3 maxCorner = GetTransfromedPoint(maxVert, transform);

		vec3 otherMinCorner = GetTransfromedPoint(box_->maxVert, box_->transform);
		vec3 otherMaxCorner = GetTransfromedPoint(box_->maxVert, box_->transform);

		if( minCorner.x <= otherMaxCorner.x && maxCorner.x >= otherMinCorner.x &&
			minCorner.y <= otherMaxCorner.y && maxCorner.y >= otherMinCorner.y &&
			minCorner.z <= otherMaxCorner.z && maxCorner.z >= otherMinCorner.z)
		{
			return true;
		}
		else
			return false;
	}

private:
	inline vec3 GetTransfromedPoint(vec3 point_, mat4 transform_) {
		return vec3(transform_[3].x, transform_[3].y, transform_[3].z) + point_;
	}

};
#endif // !BOUNDINGBOX_H

