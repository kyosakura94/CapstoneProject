#ifndef ANIMATOR_H
#define ANIMATOR_H


#include "Animation.h"
#include "../../Core/Timer.h"
#include "../animatedModel/Joint.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../animatedModel/AnimatedModel.h"
#include "../../Rendering/3D/Component.h"
#include "../../Rendering/3D/GameObject.h"

using namespace glm;
enum class Anim {IDLE, RUN, DIE};

class Animator : public Component
{

private:
	Anim currentAnim;
	Animation* currentAnimation;
	Animation_B * currentAnimation_B;
	float animationTime = 0;
	Joint rootJoint;
	int check;
	vector<mat4> check_;
	mat4 globalInverseTransform;
	std::vector<glm::mat4> currentPose;
	GameObject* gameObject;
	float totalTime;
	vector<Animation_B*> listofAnimation;
	bool isDeath = false;

public:

	Animator();
	virtual ~Animator();
	virtual bool OnCreate(GameObject* parent_);
	virtual void Update(float deltaTime_);
	virtual bool delayedUpdate();

	void setDeadth(bool d) { isDeath = d; }
	void pushAnimation(Animation_B * ani);
	void runAnimation(int index_);
	Anim getCurrentAniState() { return currentAnim; }
	void setCurrentAniState(Anim v_) { currentAnim = v_; }
	std::pair<uint, float> getTimeFraction(std::vector<float>& times, float& dt);
	bool isDone = false;
	bool isDieAnimationDone = false;
	int anieindex_ = 0;
	void setAnimation(Animation* animation_) { currentAnimation = animation_; }

	void setAnimation(Animation_B* animation_);

	void getPose(Animation_B* animation, Joint& skeletion, float dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform);
	
	void update(float deltaTime_){};


	void doAnimation(Animation_B* animation);

	void increaseAnimationTime(float deltatime_) {

		animationTime += deltatime_;
		cout << animationTime << endl;
		if (animationTime > currentAnimation->getLength()) {
			animationTime = fmod(animationTime, currentAnimation->getLength());
		}
	}
	unordered_map<string, mat4> calculateCurrentAnimationPose()
	{
		vector<KeyFrame> frames = getPreviousAndNextFrames();
		float progression = calculateProgression(frames[0], frames[1]);
		return interpolatePoses(frames[0], frames[1], progression);
	}

	vector<KeyFrame> getPreviousAndNextFrames()
	{
		vector<KeyFrame> allFrames = currentAnimation->getKeyFrames();
		KeyFrame previousFrame = allFrames[0];
		KeyFrame nextFrame = allFrames[0];

		for (int i = 1; i < allFrames.size(); i++)
		{
			nextFrame = allFrames[i];
			if (nextFrame.getTimeStamp() > animationTime)
			{
				break;
			}
			previousFrame = allFrames[i];
		}

		return vector<KeyFrame>{ previousFrame, nextFrame };
	}

	float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame) {
		float totalTime = nextFrame.getTimeStamp() - previousFrame.getTimeStamp();
		float currentTime = animationTime - previousFrame.getTimeStamp();
		return currentTime / totalTime;
	}

	unordered_map<string, mat4> interpolatePoses(KeyFrame& previousFrame, KeyFrame& nextFrame, float progression)
	{
		unordered_map<string, mat4> currentPose;

		unordered_map<string, JointTransform> test = previousFrame.getJointKeyFrames();

		for (auto& pose : previousFrame.getJointKeyFrames())
		{

			JointTransform previousTransform = previousFrame.getJointKeyFrames().at(pose.first);

			JointTransform nextTransform = nextFrame.getJointKeyFrames().at(pose.first);

			JointTransform currentTransform = JointTransform::interpolate(previousTransform, nextTransform, progression);

			mat4 localTransform = currentTransform.getLocalTransform();

			std::pair<std::string, mat4> pair(pose.first, localTransform);

			currentPose.insert(pair);
		}

		return currentPose;
	}

	void applyPoseToJoints(unordered_map<string, mat4> currentPose, Joint& joint, mat4 parentTransform)
	{
		string name = joint.name;
		mat4 currentLocalTransform = currentPose[joint.name];
		mat4 currentTransform = parentTransform * currentLocalTransform;

		currentTransform = globalInverseTransform * currentTransform * joint.offset;
		joint.animatedTransform = currentTransform;
		check_.push_back(currentTransform);

		for (Joint childJoint : joint.children) {
			applyPoseToJoints(currentPose, childJoint, currentTransform);
		}
	}
};



#endif // !ANIMATOR_H
