#include "Physics.h"

vec3 previous_accel, current_accel;

void Physics::SimpleNewtonMotion(GameObject &model, const float deltTime_)
{
	quat angularVelocity(0.0f, model.GetAngVel().x, model.GetAngVel().y, model.GetAngVel().z);

	//Quaternion angularVelocity(0.0f, model.GetAngVel().x, model.GetAngVel().y, model.GetAngVel().z);

	previous_accel = model.GetAccel();

	model.SetPosition(model.GetPosition() += model.GetVelocity() * deltTime_ + (previous_accel * 0.5f * deltTime_ * deltTime_));
	model.SetVelocity(model.GetVelocity() += 0.5f * (previous_accel + current_accel) * deltTime_);


	quat finalQua = model.getQuaternion() += 0.5f * (angularVelocity * model.getQuaternion() * deltTime_);
	model.SetQuaternion(normalize(finalQua));


	previous_accel = current_accel ;
}

quat Physics::RotationBetweenVectors(vec3 start, vec3 dest)
{
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (length(rotationAxis) < 0.01f) // bad luck, they were parallel, try again!
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);
		
		rotationAxis = normalize(rotationAxis);
		return quat(glm::radians(180.0f), rotationAxis);
		
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

vec3 Physics::ComputeForwardVector(quat q_)
{
	const float x2 = 2.0f * q_.x;
	const float y2 = 2.0f * q_.y;
	const float z2 = 2.0f * q_.z;
	const float x2w = x2 * q_.w;
	const float y2w = y2 * q_.w;
	const float x2x = x2 * q_.x;
	const float z2x = z2 * q_.x;
	const float y2y = y2 * q_.y;
	const float z2y = z2 * q_.y;
	return vec3(z2x + y2w, z2y - x2w, 1.0f - (x2x + y2y));
}

quat Physics::RotateTowards(quat q1, quat q2, float maxAngle)
{
	if (maxAngle < 0.001f) {
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}

	float cosTheta = dot(q1, q2);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if (cosTheta > 0.9999f) {
		return q2;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0) {
		q1 = q1 * -1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2&deg; difference, and we are allowed 5&deg;,
	// then we arrived.
	if (angle < maxAngle) {
		return q2;
	}

	float fT = maxAngle / angle;
	angle = maxAngle;

	quat res = (sin((1.0f - fT) * angle) * q1 + sin(fT * angle) * q2) / sin(angle);
	res = normalize(res);
	return res;
}



//bool Physics::SphereSphereCollision(const GameObject & model1, const GameObject & model2)
//{
//	float distance = VMath::distance(model1.position,model2.position);
//
//	if (distance < model1.radius + model2.radius )
//	{
//		return true;
//	}
//	return false;
//}

//void Physics::SphereSphereCollisionRespone(GameObject &model1, GameObject &model2){
//	
//	vec3 n = normalize(model1.GetPosition() - model2.GetPosition());
//
//	float v1p = dot(model1.GetVelocity(), n); 
//	float v2p = VMath::dot(model2.vel, n);
//
//	float m1 = model1.mass * VMath::mag(model1.vel);
//	float m2 = model2.mass * VMath::mag(model2.vel);
//	float k = m1 + m2;
//	float e = 1.0;
//
//	float v1pp = ((m1 - e * m2) * v1p + (1 + e) * m2 * v2p) / k;
//	float v2pp = ((m2 - e * m1) * v2p + (1 + e) * m1 * v1p) / k;
//
//	float t = VMath::distance(model1.GetPosition(), model2.GetPosition());
//	float f = model1.radius + model2.radius;
//	float d = f - t;
//
//	if (d <= 0)
//	{
//		model1.GetPosition() += (-d * n * 0.5);
//		model2.GetPosition() += ( d * n * 0.5);
//	}
//
//	model1.GetVelocity() += (v1pp - v1p) * n;
//	model2.GetVelocity() += (v2pp - v2p) * n;
//}
//
//void Physics::SphereStaticSphereCollisionResponse(GameObject & model1, const GameObject & staticSphere)
//{
//	float t = VMath::distance(model1.GetPosition(), staticSphere.GetPosition());
//	float k = model1.radius + staticSphere.radius;
//	float d = k - t;
//
//	vec3 n = VMath::normalize(model1.GetPosition() - staticSphere.GetPosition());
//	vec3 vp = VMath::dot(-model1.vel, n) *  n;
//
//	model1.vel += 2 * vp;
//	if (d <= 0)
//	{
//		model1.GetPosition() += (-d * n);
//	}
//
//}
//
//void Physics::ApplyForce(GameObject &model1, vec3 force)
//{
//		// Code up Newton's second law F = ma to update acceleration
//		// a = F / m
//		model1.GetAccel = force / model1.mass;
//}
//void Physics::ApplyGravity(GameObject & model1)
//{
//	vec3 gravity = vec3(0.0, -9.8, 0.0);
//	vec3 fNet;
//	fNet = model1.mass * gravity;
//	ApplyForce(model1, fNet);
//}
//void Physics::ApplyForces(GameObject &model1, float waterHeight)
//{
//	vec3 gravity = vec3(0.0, -9.8, 0.0);
//	float coefficient = 1.0f;
//	float liquidDensity = 1.0f;
//
//	vec3 finalForces;
//	vec3 fBouy;
//	vec3 fNet;
//	vec3 fDrag;
//
//	fNet = model1.mass * gravity;
//	fDrag = -coefficient * model1.GetVelocity();
//
//	float v = 0;
//	float totalSubmerged = (M_PI * pow(model1.radius, 2) * waterHeight);
//
//	float m = (model1.GetPosition().y + model1.radius) - waterHeight;
//
//	if ( m >= model1.radius)
//	{
//		//totall submerged
//		if (m < waterHeight)
//		{
//			v = totalSubmerged;
//		}
//		else
//		{
//			v = 0;
//		}
//	}
//	else
//	{
//		float angle;
//		float k = m / model1.radius;
//		
//		if (k <= 1 && k >= -1)
//		{
//			angle = 2.0f * acos(k);
//		}
//		else
//		{
//			angle = 0;
//		}
//
//		float pi = angle - sin(angle);
//		float v0 = 0.5f * pow(model1.radius, 2) * pi * waterHeight;
//
//		if (model1.GetPosition().y > waterHeight)
//		{
//			v = totalSubmerged - v0;
//			gravity.y = 9.8;
//		}
//		else
//		{
//			v = v0;
//			gravity.y = 9.8;
//		}
//	}
//
//	fBouy = liquidDensity * gravity * v;
//
//	finalForces = fNet + fDrag + fBouy;
//
//	ApplyForce(model1, finalForces);
//}
//
//bool Physics::SpherePlaneCollisionDetected(const GameObject & model1, const Plane & plane)
//{
//	vec4 bodyPosition = vec4(model1.GetPosition());
//
//	float distance = bodyPosition.Dot(plane) - model1.radius;
//
//	if (distance < 0.0f) {
//		return true;
//	}
//	return false;
//}
//
//void Physics::SpherePlaneCollisionResponse(GameObject & model1, const Plane & plane)
//{
//	vec4 bodyPosition = vec4(model1.GetPosition());
//
//	float d = bodyPosition.Dot(plane) - model1.radius;
//
//	vec3 normal = vec3(plane.x, plane.y, plane.z);
//
//	vec3 n = VMath::normalize(normal);
//
//	vec3 vp = VMath::dot(-model1.GetVelocity(), n) *  n;
//
//	model1.GetVelocity() += 2 * vp;
//
//	if (d <= 0)
//	{
//		model1.GetPosition() += (-d * n);
//	}
//}
