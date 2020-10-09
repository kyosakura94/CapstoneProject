#pragma once
#include "../../Rendering/3D/GameObject.h"
#include "Plane.h"

class Physics
{

public:
	static void SimpleNewtonMotion(GameObject &model ,const float deltTime_ );
	static bool SphereSphereCollision(const GameObject &model1, const GameObject &model2);
	static void SphereSphereCollisionRespone(GameObject &model1,GameObject &model2);
	static void SphereStaticSphereCollisionResponse(GameObject &model1, const GameObject &staticSphere);
	static void ApplyForce(GameObject &model1, vec3 force);

	static void ApplyGravity(GameObject &model1);

	static void ApplyForces(GameObject &model1, float waterHeight);
	static bool SpherePlaneCollisionDetected(const GameObject &model1, const Plane &plane);
	static void SpherePlaneCollisionResponse(GameObject &model1, const Plane &plane);

};

