#pragma once
#include "../../Rendering/3D/GameObject.h"
#include "Plane.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
class Physics
{

public:
	static void SimpleNewtonMotion(GameObject &model ,const float deltTime_ );
	static quat RotationBetweenVectors(vec3 start, vec3 dest);
	static vec3 ComputeForwardVector(quat q_);
	static quat RotateTowards(quat q1, quat q2, float maxAngle);


	static bool SphereSphereCollision(const GameObject &model1, const GameObject &model2);
	static void SphereSphereCollisionRespone(GameObject &model1,GameObject &model2);
	static void SphereStaticSphereCollisionResponse(GameObject &model1, const GameObject &staticSphere);
	static void ApplyForce(GameObject &model1, vec3 force);

	static void ApplyGravity(GameObject &model1);

	static void ApplyForces(GameObject &model1, float waterHeight);
	static bool SpherePlaneCollisionDetected(const GameObject &model1, const Plane &plane);
	static void SpherePlaneCollisionResponse(GameObject &model1, const Plane &plane);

};

