#include "Animator.h"


Animator::~Animator()
{
}

bool Animator::OnCreate(GameObject* parent_)
{
	parent = parent_;
	rootJoint = parent->getModel()->getRootJoint();
	//NEED TO GET THE ANIMATION
	setAnimation(parent->getModel()->getLoadOBJModel()->getAnimation());
	return true;
}

void Animator::Update(float deltaTime_)
{
	totalTime += deltaTime_;
	// elapsedTime = (float)SDL_GetTicks() / 1000;
	glm::mat4 identity(1.0);
	glm::mat4 global = parent->getModel()->getglobalInverseTransform();

	getPose(currentAnimation_B, rootJoint, totalTime, currentPose, identity, global);

	parent->getModel()->setCurrentPose(currentPose);
}

bool Animator::delayedUpdate()
{
	return false;
}

void Animator::pushAnimation(Animation_B * ani)
{
	if (ani != nullptr)
	{
		listofAnimation.push_back(ani);
	}
}

void Animator::runAnimation(int index_)
{
	if (listofAnimation.size() >= 0)
	{
		switch (index_)
		{
		case 1:
			currentAnim = Anim::RUN;
			break;		
		
		case 0:
			currentAnim = Anim::IDLE;
			break;		
		
		case 3:
			currentAnim = Anim::DIE;
			break;
		default:
			break;
		}
		anieindex_ = index_;
		animationTime = 0;
		currentAnimation_B = listofAnimation[index_];
	}
}

std::pair<uint, float> Animator::getTimeFraction(std::vector<float>& times, float& dt)
{
	uint segment = 0;

	while (dt >= times[segment])
		segment++;

	float start;
	if (segment != 0)
	{
		start = times[segment - 1];
	}
	else
	{
		segment = 1;
		start = times[0];
	}

	float end = times[segment];
	float frac;
	float k = end - start;
	if (k <= 0)
	{
		frac = 0;
	}
	else
	{
		frac = (dt - start) / fabs(end - start);
	}

	return { segment, frac };
}

void Animator::setAnimation(Animation_B* animation_)
{
	glm::mat4 identity(1.0);
	currentPose = {};
	currentPose.resize(parent->getModel()->getJointCount(), identity);
	currentAnimation_B = animation_;
}

Animator::Animator()
{

}


void Animator::getPose(Animation_B* animation, Joint& skeletion, float dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform)
{

	BoneTransformTrack& btt = animation->boneTransforms[skeletion.name];
	dt = fmod(dt, animation->duration);

	std::pair<uint, float> fp;
	//calculate interpolated position
	if (btt.positionTimestamps.size() != 0)
	{
		fp = getTimeFraction(btt.positionTimestamps, dt);

		//cout << fp.first << endl;
		if (fp.first >= (btt.positionTimestamps.size() - 1))
		{
			isDone = true;

			if (anieindex_ == 3 && isDone == true)
			{
				isDieAnimationDone = true;
			}
		}

		glm::vec3 position1 = btt.positions[fp.first - 1];
		glm::vec3 position2 = btt.positions[fp.first];

		glm::vec3 position = glm::mix(position1, position2, fp.second);

		//calculate interpolated rotation
		fp = getTimeFraction(btt.rotationTimestamps, dt);
		glm::quat rotation1 = btt.rotations[fp.first - 1];
		glm::quat rotation2 = btt.rotations[fp.first];

		glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);

		//calculate interpolated scale
		fp = getTimeFraction(btt.scaleTimestamps, dt);
		glm::vec3 scale1 = btt.scales[fp.first - 1];
		glm::vec3 scale2 = btt.scales[fp.first];

		glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

		glm::mat4 positionMat = glm::mat4(1.0), scaleMat = glm::mat4(1.0);


		// calculate localTransform
		positionMat = glm::translate(positionMat, position);

		//glm::mat4 rotationMat = glm::toMat4(rotation) ;
		glm::mat4 rotationMat = glm::toMat4(rotation);

		scaleMat = glm::scale(scaleMat, scale);


		glm::mat4 localTransform = positionMat * rotationMat;// *scaleMat;
		glm::mat4 globalTransform = parentTransform * localTransform;

		mat4 finaltransform = globalInverseTransform * globalTransform * skeletion.offset;

		mat4 correctTransform;

		mat4 model;
			
		mat4 test = rotate(correctTransform, glm::radians(180.0f), vec3(1, 0, 0));

		model = glm::scale(model, vec3(0.001f));

		output[skeletion.id] = test * model * finaltransform;


		//update values for children bones

		for (Joint& child : skeletion.children) {
			getPose(animation, child, dt, output, globalTransform, globalInverseTransform);
		}

	}


}

void Animator::doAnimation(Animation_B* animation)
{
	currentAnim = Anim::IDLE;
	animationTime = 0;
	currentAnimation_B = animation;
}
