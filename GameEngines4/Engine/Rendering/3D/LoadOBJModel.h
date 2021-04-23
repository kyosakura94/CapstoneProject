
#ifndef LOADOBJMODEL_H
#define LOADOBJMODEL_H

#include "OpenGLMesh.h"
#include "../../Core/Debug.h"
#include "../../FX/MaterialLoader.h"
#include <sstream>
#include "../../Math/BoundingBox.h"
#include "../../Animation/animation_sub/Animation.h"
#include "../../Animation/animatedModel/Joint.h"
#include "../../Animation/utils.h"
#include <unordered_map>
#include "../../Animation/animation_sub/KeyFrame.h"
#include "../../Animation/animation_sub/JointTransform.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>

using namespace std;
using namespace glm;
class LoadOBJModel
{
public:
	LoadOBJModel();
	~LoadOBJModel();
	void LoadModel(const std::string& objFilePath);
	void LoadModelAssimp(const std::string& objFilePath);
	void LoadModel(const std::string& objFilePath, const std::string& matFilePath);
	std::vector<Vertex> GetVers();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubMeshes();
	BoundingBox GetBoundingBox();
	std::string getModelName() { return modelName; };
	void OnDestroy();
	vector<vec3> getvertercies() { return vertercies; };

	void LoadAnimatedModel(const std::string& objFilePath, const std::string& matFilePath);
	int getJointCount() { return nBoneCount; };
	void loadAnimation(const aiScene* scene);
	//Animation getAnimation() { return animation; }

	Animation_B *getAnimation() { return animation; }
	Joint getRootJoint() { return rootJoint; }
	glm::mat4 getGlobalInverseTransform() { return globalInverseTransform; }

private:

	vector<vec3> vertercies;
	vector<vec3> normals;
	vector<vec2> textureCoords;
	vector<int> indices;
	vector<int> nomalIndices;
	vector<int> textureIndices;
	vector<Vertex> meshVerterices;
	vector<Vertex> totalmeshVerterices;
	vector<SubMesh> subMeshes;
	std::map<std::string, unsigned int> m_BoneMapping;
	glm::mat4 globalInverseTransform;
	int nBoneCount;
	std::string modelName;
	void PostProcessing();
	void LoadMaterial(const string& fileName_);
	void LoadMaterialLibary(const string& matFilePath_);
	BoundingBox box;
	Material currentMaterial;
	Joint rootJoint;
	Animation_B *animation;
	bool readSkeleton(Joint& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable);
};
#endif // !LOADOBJMODEL_H


