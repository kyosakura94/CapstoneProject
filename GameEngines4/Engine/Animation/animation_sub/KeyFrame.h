#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "JointTransform.h"
#include <iostream>

using namespace std;
using namespace glm;
class KeyFrame
{
private:
	float timeStamp;
	unordered_map<string, JointTransform> pose;
public:

	KeyFrame();
	KeyFrame(float timeStamp, unordered_map<string, JointTransform> jointKeyFrames);
	~KeyFrame();

	float getTimeStamp() {
		return timeStamp;
	}

	unordered_map<string, JointTransform> getJointKeyFrames() 
	{
		return pose;
	}
};

