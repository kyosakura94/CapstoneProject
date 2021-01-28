#ifndef MODEL_H
#define MODEL_H

#include "OpenGLMesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "LoadOBJModel.h"
#include "../Renderer/Renderer.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;
using namespace glm;
class Model
{
public:
	static enum { OUTSIDE, INTERSECT, INSIDE };
	Model(const string& objFilepath, const string& matFilepath, GLuint shaderProgram_, RendererType rendererType_);
	~Model();
	void Render(Camera* camera_, string tag);
	void AddMesh(OpenGLMesh* mesh_);
	int CreateInstance(vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	void UpdateInstance(int index_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	BoundingBox GetBoundingBox();
	GLuint getShaderProgram();
	mat4 GetTransform(int index_) const;
	int AABBandFrustum(Camera* camera);
	string getModelName() { return modelName; }
	void setModelName(string name_) { modelName = name_; }
	std::vector<OpenGLMesh*> getsubMesh() { return subMesh; };
	LoadOBJModel* getLoadOBJModel();
	vector<vec3> getvertercies(int index_);

private:
	std::vector<OpenGLMesh*> subMesh;
	GLuint shaderProgram;
	glm::mat4 GetTransform(vec3 position_, float angle_, vec3 rotation_, vec3 scale_) const;
	void LoadModel();
	vector<mat4> modelInstances;
	vector<vec3> vertercies;
	vector<vec3> worldVertecies;
	LoadOBJModel* obj;
	BoundingBox box;
	RendererType rendererType;
	string modelName;
};
#endif // !MODEL_H
