#pragma once

#include "KeyFrame.h"
#include <vector>
#include <unordered_map>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../../Animation/utils.h"

using namespace std;

class Animation
{
public:

	Animation();
	Animation(float lengthInSeconds, vector<KeyFrame> frames);
	~Animation();

	float getLength() { return length; }
	vector<KeyFrame> getKeyFrames() { return keyFrames; }

	void loadAnimation();

private:
	float length;//in seconds
	vector<KeyFrame> keyFrames;
};

struct BoneTransformTrack
{
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<glm::vec3> positions = {};
	std::vector<glm::quat> rotations = {};
	std::vector<glm::vec3> scales = {};
};

struct Bone {
	int id = 0; // position of the bone in final upload array
	std::string name = "";
	glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children = {};
};

struct Animation_B 
{
	void loadAnimation(const std::string& objFilePath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(objFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

		if (scene->mAnimations[0] == nullptr)
		{
			return;
		}

		aiAnimation* anim = scene->mAnimations[0];

		if (anim->mTicksPerSecond != 0.0f)
			this->ticksPerSecond = anim->mTicksPerSecond;
		else
			this->ticksPerSecond = 1;

		this->duration = anim->mDuration * anim->mTicksPerSecond;
		this->boneTransforms = {};

		//load positions rotations and scales for each bone
		// each channel represents each bone

		for (int i = 0; i < anim->mNumChannels; i++) {
			aiNodeAnim* channel = anim->mChannels[i];
			BoneTransformTrack track;
			for (int j = 0; j < channel->mNumPositionKeys; j++) {
				track.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
				track.positions.push_back(assimpToGlmVec3(channel->mPositionKeys[j].mValue));
			}
			for (int j = 0; j < channel->mNumRotationKeys; j++) {
				track.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
				track.rotations.push_back(assimpToGlmQuat(channel->mRotationKeys[j].mValue));

			}
			for (int j = 0; j < channel->mNumScalingKeys; j++) {
				track.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
				track.scales.push_back(assimpToGlmVec3(channel->mScalingKeys[j].mValue));

			}

			this->boneTransforms[channel->mNodeName.C_Str()] = track;
		}
	};


	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
};

