#include "LoadOBJModel.h"



LoadOBJModel::LoadOBJModel() : currentMaterial(Material()), nBoneCount(0)
{
	nBoneCount = 0;
	vertercies.reserve(200);
	normals.reserve(200);
	textureCoords.reserve(200);
	indices.reserve(200);
	nomalIndices.reserve(200);
	textureIndices.reserve(200);
	meshVerterices.reserve(200);
	subMeshes.reserve(10);
	animation = new Animation_B();
}


LoadOBJModel::~LoadOBJModel()
{
	OnDestroy();
}

void LoadOBJModel::LoadModel(const std::string & objFilePath)
{
	float maxX = -9999.99f;
	float maxY = -9999.99f;
	float maxZ = -9999.99f;
	float minX = 9999.99f;
	float minY = 9999.99f;
	float minZ = 9999.99f;

	ifstream in(objFilePath.c_str(), ios::in);
	if (!in)
	{
		Debug::Error("Cannot open OBJ file:" + objFilePath, "LoadOBJModel.cpp", __LINE__);
		return;
	}
	string line;
	while (getline(in, line))
	{
		//VERTEX DATA
		if (line.substr(0, 2) == "v ")
		{
			istringstream v(line.substr(2));
			float x, y, z;
			v >> x >> y >> z;
			vertercies.push_back(vec3(x, y, z));
			if (x > maxX)
			{
				maxX = x;
			}
			if (y > maxY)
			{
				maxY = y;
			}
			if (z > maxZ)
			{
				maxZ = z;
			}
			if (x < minX)
			{
				minX = x;
			}
			if (y < minY)
			{
				minY = y;
			}
			if (z < minZ)
			{
				minZ = z;
			}
			box.maxVert = vec3(maxX, maxY, maxZ);
			box.minVert = vec3(minX, minY, minZ);
		}
		//NORMAL DATA
		else if (line.substr(0, 3) == "vn ")
		{
			istringstream v(line.substr(3));
			float x, y, z;
			v >> x >> y >> z;
			normals.push_back(vec3(x, y, z));
		}
		//TEXTURE COODINATE DATA
		else if (line.substr(0, 3) == "vt ")
		{
			istringstream v(line.substr(3));
			float x, y, z;
			v >> x >> y >> z;
			textureCoords.push_back(vec2(x, y));

		}
		//FACE DATA
		else if (line.substr(0, 2) == "f ")
		{
			istringstream v(line.substr(2));
			string word;
			int x[3], y[3], z[3];
			int i = 0;
			char sep;

			while (v>>word)
			{
				istringstream ss(word);
				ss >> x[i] >> sep >> y[i] >> sep >> z[i];
				indices.push_back(x[i] - 1);
				textureIndices.push_back(y[i] - 1);
				nomalIndices.push_back(z[i] - 1);
				i++;
			}
		}

		else if (line.substr(0, 7) == "usemtl ")
		{
			if (indices.size() > 0)
			{
				PostProcessing();
			}

			LoadMaterial(line.substr(7));
		}
		else if (line.substr(0, 7) == "mtllib " )
		{
			modelName = line.substr(7);
		}
	}

	PostProcessing();
	
}

void LoadOBJModel::LoadModelAssimp(const std::string& objFilePath)
{
	float maxX = -9999999.99f;
	float maxY = -9999999.99f;
	float maxZ = -9999999.99f;
	float minX = 9999999.99f;
	float minY = 9999999.99f;
	float minZ = 9999999.99f;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(objFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	for (size_t k = 0; k < scene->mNumMeshes; k++)
	{
		aiMesh* mesh = scene->mMeshes[k];

		//load position, normal, uv
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			//process position 
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			vertercies.push_back(vector);

			if (vector.x > maxX)
			{
				maxX = vector.x;
			}
			if (vector.y > maxY)
			{
				maxY = vector.y;
			}
			if (vector.z > maxZ)
			{
				maxZ = vector.z;
			}
			if (vector.x < minX)
			{
				minX = vector.x;
			}
			if (vector.y < minY)
			{
				minY = vector.y;
			}
			if (vector.z < minZ)
			{
				minZ = vector.z;
			}
			box.maxVert = vec3(maxX, maxY, maxZ);
			box.minVert = vec3(minX, minY, minZ);

			//process normal
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			//process uv
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;

			vertex.boneIds = glm::ivec4(0);
			vertex.boneWeights = glm::vec4(0.0f);

			meshVerterices.push_back(vertex);
		}

		//load indices
		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}


		SubMesh subMesh;
		subMesh.vertexList = meshVerterices;
		subMesh.meshIndices = indices;
		subMesh.material = currentMaterial;

		subMeshes.push_back(subMesh);
		meshVerterices.clear();
	}

	indices.clear();
	nomalIndices.clear();
	textureIndices.clear();
	currentMaterial = Material();
}

void LoadOBJModel::LoadModel(const std::string & objFilePath, const std::string & matFilePath)
{
	LoadMaterialLibary(matFilePath);
	LoadModel(objFilePath);
	//LoadModelAssimp(objFilePath);
}

std::vector<Vertex> LoadOBJModel::GetVers()
{
	return meshVerterices;
}

std::vector<int> LoadOBJModel::GetIndices()
{
	return indices;
}

std::vector<SubMesh> LoadOBJModel::GetSubMeshes()
{
	return subMeshes;
}

BoundingBox LoadOBJModel::GetBoundingBox()
{
	return box;
}

void LoadOBJModel::OnDestroy()
{
		vertercies.clear();
		normals.clear();
		textureCoords.clear();
		indices.clear();
		nomalIndices.clear();
		textureIndices.clear();
		meshVerterices.clear();
		subMeshes.clear();
}

void LoadOBJModel::LoadAnimatedModel(const std::string& objFilePath, const std::string& matFilePath)
{
	float maxX = -9999999.99f;
	float maxY = -9999999.99f;
	float maxZ = -9999999.99f;
	float minX = 9999999.99f;
	float minY = 9999999.99f;
	float minZ = 9999999.99f;

	LoadMaterialLibary(matFilePath);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(objFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (scene) {

		globalInverseTransform = assimpToGlmMatrix(scene->mRootNode->mTransformation);
		globalInverseTransform = glm::inverse(globalInverseTransform);
		globalInverseTransform /= 100;
	}
	else {
		printf("Error parsing '%s': '%s'\n", objFilePath.c_str(), importer.GetErrorString());
	}

	modelName = scene->mRootNode->mName.C_Str();
	LoadMaterial(scene->mRootNode->mName.C_Str());

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;
	//load joint to vertices
	std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};

	//for (unsigned int i = 0; i < scene->mNumMeshes; i++) 
	//{
	//	// Increment total vertices and indices. 
	//	NumVertices += scene->mMeshes[i]->mNumVertices;
	//}


	for (size_t k = 0; k < scene->mNumMeshes; k++)
	{
		aiMesh* mesh = scene->mMeshes[k];

		//load position, normal, uv
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			//process position 
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			vertercies.push_back(vector);

			if (vector.x > maxX)
			{
				maxX = vector.x;
			}
			if (vector.y > maxY)
			{
				maxY = vector.y;
			}
			if (vector.z > maxZ)
			{
				maxZ = vector.z;
			}
			if (vector.x < minX)
			{
				minX = vector.x;
			}
			if (vector.y < minY)
			{
				minY = vector.y;
			}
			if (vector.z < minZ)
			{
				minZ = vector.z;
			}
			box.maxVert = vec3(maxX, maxY, maxZ);
			box.minVert = vec3(minX, minY, minZ);

			//process normal
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			//process uv
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = vec;

			vertex.boneIds = glm::ivec4(0);
			vertex.boneWeights = glm::vec4(0.0f);

			meshVerterices.push_back(vertex);
			totalmeshVerterices.push_back(vertex);
		}
	
		if (k == 0)
		{
			NumVertices = 0;
		}
		else
		{
			NumVertices += scene->mMeshes[k-1]->mNumVertices;
		}
		

		//Create boneCount Vector
		std::vector<uint> boneCounts;
		boneCounts.resize(meshVerterices.size(), 0);

		//loop through each bone
		for (uint i = 0; i < mesh->mNumBones; i++)
		{
			unsigned int BoneIndex = 0;
			aiBone* bone = mesh->mBones[i];

			// Obtain the bone name.
			std::string BoneName(mesh->mBones[i]->mName.data);

			// If bone isn't already in the map. 
			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {

				// Set the bone ID to be the current total number of bones. 
				BoneIndex = nBoneCount;

				// Increment total bones. 
				nBoneCount++;
			}
			else {
				// Bone ID is already in map. 
				BoneIndex = m_BoneMapping[BoneName];
			}

			// Obtains the offset matrix which transforms the bone from mesh space into bone space.
			mat4 m = assimpToGlmMatrix(bone->mOffsetMatrix);
			boneInfo[bone->mName.C_Str()] = { i, m };


			//loop through each vertex that have that bone
			for (int j = 0; j < bone->mNumWeights; j++)
			{
				uint id;
				//if (k == 0)
				//{
				//	id = 0 + bone->mWeights[j].mVertexId;
				//}
				//else
				//{
				//	id = NumVertices + bone->mWeights[j].mVertexId;
				//}

				id = bone->mWeights[j].mVertexId;
				float weight = bone->mWeights[j].mWeight;

				boneCounts[id]++;

				// Iterate over all the affected vertices by this bone i.e weights. 
				// Insert bone data for particular vertex ID. A maximum of 4 bones can influence the same vertex. 
				switch (boneCounts[id]) 
				{
					case 1:
						meshVerterices[id].boneIds.x = i;
						meshVerterices[id].boneWeights.x = weight;
						break;
					case 2:
						meshVerterices[id].boneIds.y = i;
						meshVerterices[id].boneWeights.y = weight;
						break;
					case 3:
						meshVerterices[id].boneIds.z = i;
						meshVerterices[id].boneWeights.z = weight;
						break;
					case 4:
						meshVerterices[id].boneIds.w = i;
						meshVerterices[id].boneWeights.w = weight;
						break;
					default:
						//std::cout << "err: unable to allocate bone to vertex" << std::endl;
						break;
				}
			}
		}

		//normalize weights to make all weights sum 1
		for (int i = 0; i < meshVerterices.size(); i++) {
			glm::vec4& boneWeights = meshVerterices[i].boneWeights;
			float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
			if (totalWeight > 0.0f) {
				meshVerterices[i].boneWeights = glm::vec4(
					boneWeights.x / totalWeight,
					boneWeights.y / totalWeight,
					boneWeights.z / totalWeight,
					boneWeights.w / totalWeight
				);
			}
		}

		//load indices
		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}


		SubMesh subMesh;
		subMesh.vertexList = meshVerterices;
		subMesh.meshIndices = indices;
		subMesh.material = currentMaterial;

		subMeshes.push_back(subMesh);

		meshVerterices.clear();
	}

    nBoneCount = boneInfo.size();
	//readSkeleton
	readSkeleton(rootJoint, scene->mRootNode, boneInfo);

	indices.clear();
	nomalIndices.clear();
	textureIndices.clear();
	currentMaterial = Material();
}

void LoadOBJModel::loadAnimation(const aiScene* scene)
{
	aiAnimation* anim = scene->mAnimations[0];

	if (anim->mTicksPerSecond != 0.0f)
		animation->ticksPerSecond = anim->mTicksPerSecond;
	else
		animation->ticksPerSecond = 1;

	animation->duration = anim->mDuration * anim->mTicksPerSecond;
	animation->boneTransforms = {};

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

		animation->boneTransforms[channel->mNodeName.C_Str()] = track;
	}

}

void LoadOBJModel::PostProcessing()
{
	for (int i = 0; i < indices.size(); i++)
	{
		Vertex vert;
		vert.position = vertercies[indices[i]];
		vert.normal = normals[nomalIndices[i]];
		vert.uv = textureCoords[textureIndices[i]];
		meshVerterices.push_back(vert);
	}

	SubMesh subMesh;
	subMesh.vertexList = meshVerterices;
	subMesh.meshIndices = indices;
	subMesh.material = currentMaterial;

	subMeshes.push_back(subMesh);

	indices.clear();
	nomalIndices.clear();
	textureIndices.clear();
	meshVerterices.clear();
	currentMaterial = Material();

}

void LoadOBJModel::LoadMaterial(const string & fileName_)
{
	currentMaterial = MaterialHandler::getInstance()->GetMaterial(fileName_);
}

void LoadOBJModel::LoadMaterialLibary(const string & matFilePath_)
{
	MaterialLoader::loadMaterial(matFilePath_);
}

bool LoadOBJModel::readSkeleton(Joint& jointOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable)
{
	if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end())
	{ 
		// if node is actually a bone
		jointOutput.name = node->mName.C_Str();
		jointOutput.id = boneInfoTable[jointOutput.name].first;
		jointOutput.localBindTransform = boneInfoTable[jointOutput.name].second;
		jointOutput.offset = boneInfoTable[jointOutput.name].second;

		for (int i = 0; i < node->mNumChildren; i++) {
			Joint child;
			readSkeleton(child, node->mChildren[i], boneInfoTable);
			jointOutput.children.push_back(child);
		}
		return true;
	}
	else { // find bones in children
		for (int i = 0; i < node->mNumChildren; i++)
		{
			if (readSkeleton(jointOutput, node->mChildren[i], boneInfoTable)) {
				return true;
			}
		}
	}
	return false;
}


/*
//CHUNK CODE

	//aiAnimation* anim = scene->mAnimations[0];

	//double ticksPerSecond;
	//
	//if (anim->mTicksPerSecond != 0.0f)
	//	ticksPerSecond = anim->mTicksPerSecond;
	//else
	//	ticksPerSecond = 1;

	//vector<KeyFrame> keyFrames;

	//for (size_t i = 0; i < 5; i++)
	//{
	//	unordered_map<string, JointTransform> pose;
	//	for (size_t j = 0; j < anim->mNumChannels; j++)
	//	{
	//		aiNodeAnim* channel = anim->mChannels[j];

	//		vec3 pos = (assimpToGlmVec3(channel->mPositionKeys[j].mValue));
	//		quat rotation = (assimpToGlmQuat(channel->mRotationKeys[j].mValue));
	//		pose.insert({ channel->mNodeName.C_Str(), JointTransform(pos, rotation) });
	//	}

	//	aiNodeAnim* timeChannel = anim->mChannels[0];
	//	float timestamp = timeChannel->mPositionKeys[i].mTime;
	//
	//	KeyFrame key = KeyFrame(timestamp, pose);
	//	keyFrames.push_back(key);
	//}

	////create new animation for the model

	////set length
	//float length = anim->mDuration * anim->mTicksPerSecond;

	//animation = Animation(length, keyFrames);
	//loading  first Animation

*/