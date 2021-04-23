#pragma once

#include "../../Rendering/3D/Model.h"
#include "Joint.h"
#include "../animation_sub/Animation.h"


class Animator;

class AnimatedModel : public Model
{
public:
	
	AnimatedModel();
	AnimatedModel(Joint rootJoint_, int jointCount_);
	AnimatedModel(const string& objFilepath_, const string& matFilepath_, GLuint shaderProgram_, RendererType rendererType_ , bool veri_);
	AnimatedModel(const string& objFilepath_, const string& matFilepath_, GLuint shaderProgram_, RendererType rendererType_ );
	
	LoadOBJModel* getLoadOBJModel() { return obj; };

	std::vector<glm::mat4> currentPose = {};

	virtual ~AnimatedModel();
	virtual void Test();
	void update(float deltaTime_);

	int getJointCount() { return jointCount; };
	
	vector<mat4> getJointTransform()
	{
		vector<mat4> jointMatrices;
		addJointsToArray(rootJoint, jointMatrices);
		return jointMatrices;
	};
	
	void addJointsToArray(Joint &headJoint, vector<mat4> &jointMatrices)
	{
		mat4 test =  headJoint.getanimatedTransform();
		jointMatrices.push_back(test);

		for (Joint childJoint : headJoint.children) {
			addJointsToArray(childJoint, jointMatrices);
		}
	}

	std::vector<glm::mat4> getCurrentPose() { return getCurrentPoseData; };


	mat4 getglobalInverseTransform() { return globalInverseTransform; };

	Joint getRootJoint()
	{
		return rootJoint;
	};
	
	void setCurrentPose(std::vector<glm::mat4> getCurrentPose_)
	{
		getCurrentPoseData = getCurrentPose_;
	};

	void doAnimation(Animation_B* animation);

	Joint rootJoint;
protected:

private:
	
	LoadOBJModel* aniObj;
	int jointCount;
	std::vector<glm::mat4> getCurrentPoseData;
	Animator * animator;
	Animation_B animation;
	mat4 globalInverseTransform;
	//void addJointsToArray(Joint headJoint, vector<mat4> &jointMatrices)
	//{
	//	jointMatrices.push_back(headJoint.getanimatedTransform());
	//	for (Joint childJoint : headJoint.children) {
	//		addJointsToArray(childJoint, jointMatrices);
	//	}
	//}
};

