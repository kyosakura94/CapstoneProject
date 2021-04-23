#include "AnimatedModel.h"
#include "../animation_sub/Animator.h"

AnimatedModel::AnimatedModel()
{
}

AnimatedModel::AnimatedModel(Joint rootJoint_, int jointCount_)
{
	rootJoint = rootJoint_;
	jointCount = jointCount_;
	mat4 identity(1.0f);
	//animator = new Animator(this);
	rootJoint.calcInsverseBindTransform(identity);
	
}

AnimatedModel::AnimatedModel(const string& objFilepath_, const string& matFilepath_, GLuint shaderProgram_, RendererType rendererType_, bool veri_)
{
	subMesh.reserve(10);
	modelInstances.reserve(5);
	modelInstances.reserve(5);
	shaderProgram = shaderProgram_;
	rendererType = rendererType_;
	obj = new LoadOBJModel();
	obj->LoadModel(objFilepath_, matFilepath_);
	modelName = obj->getModelName();

	for (auto m : obj->getvertercies())
	{
		vertercies.push_back(m);
	}


	this->LoadModel();
}

AnimatedModel::AnimatedModel(const string& objFilepath_, const string& matFilepath_, GLuint shaderProgram_, RendererType rendererType_) 
{
	subMesh.reserve(10);
	modelInstances.reserve(5);
	modelInstances.reserve(5);
	shaderProgram = shaderProgram_;
	rendererType = rendererType_;
	obj = new LoadOBJModel();
	obj->LoadAnimatedModel(objFilepath_, matFilepath_);
	modelName = obj->getModelName();
	jointCount = obj->getJointCount();
	rootJoint = obj->getRootJoint();
	//animation = obj->getAnimation();
	globalInverseTransform = obj->getGlobalInverseTransform();

	for (auto m : obj->getvertercies())
	{
		vertercies.push_back(m);
	}

	this->LoadModel();



	mat4 identity(1.0f);
	//animator = new Animator(this);
	//animator->setAnimation(animation);
	rootJoint.calcInsverseBindTransform(identity);
}

AnimatedModel::~AnimatedModel()
{
}

void AnimatedModel::Test()
{

}


void AnimatedModel::update(float deltaTime_)
{
	animator->update(deltaTime_);
}


void AnimatedModel::doAnimation(Animation_B* animation)
{
	animator->doAnimation(animation);
}
