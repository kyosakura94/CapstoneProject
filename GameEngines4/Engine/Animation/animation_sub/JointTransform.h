#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>



using namespace glm;

class JointTransform
{

private:

	vec3 position;
	quat rotation;

	static vec3 interpolate(vec3 start, vec3 end, float progression) {
		float x = start.x + (end.x - start.x) * progression;
		float y = start.y + (end.y - start.y) * progression;
		float z = start.z + (end.z - start.z) * progression;
		return vec3(x, y, z);
	}
public:
	JointTransform();
	JointTransform(vec3 position, quat rotation);
	~JointTransform();


	mat4 getLocalTransform() 
	{
		glm::mat4 positionMat;
		positionMat = glm::translate(positionMat, position);
		glm::mat4 rotationMat = glm::toMat4(rotation);
		glm::mat4 localTransform = positionMat * rotationMat;

		return localTransform;
	}
	static JointTransform interpolate(JointTransform frameA, JointTransform frameB, float progression) 
	{
		vec3 pos = interpolate(frameA.position, frameB.position, progression);
		quat rot = glm::mix(frameA.rotation, frameB.rotation, progression);
		return JointTransform(pos, rot);
	}
};

