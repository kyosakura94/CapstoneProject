
#ifndef LOADOBJMODEL_H
#define LOADOBJMODEL_H

#include "OpenGLMesh.h"
#include "../../Core/Debug.h"
#include "../../FX/MaterialLoader.h"
#include <sstream>
#include "../../Math/BoundingBox.h"

using namespace std;
using namespace glm;
class LoadOBJModel
{
public:
	LoadOBJModel();
	~LoadOBJModel();
	void LoadModel(const std::string& objFilePath);
	void LoadModel(const std::string& objFilePath, const std::string& matFilePath);
	std::vector<Vertex> GetVers();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubMeshes();
	BoundingBox GetBoundingBox();
	std::string getModelName() { return modelName; };
	void OnDestroy();
	vector<vec3> getvertercies() { return vertercies; };
private:
	vector<vec3> vertercies;
	vector<vec3> normals;
	vector<vec2> textureCoords;
	vector<int> indices;
	vector<int> nomalIndices;
	vector<int> textureIndices;
	vector<Vertex> meshVerterices;
	vector<SubMesh> subMeshes;

	std::string modelName;
	void PostProcessing();
	void LoadMaterial(const string& fileName_);
	void LoadMaterialLibary(const string& matFilePath_);
	BoundingBox box;
	Material currentMaterial;
};
#endif // !LOADOBJMODEL_H


