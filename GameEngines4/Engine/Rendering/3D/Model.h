#ifndef MODEL_H
#define MODEL_H

#include "OpenGLMesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "LoadOBJModel.h"
#include "../Renderer/Renderer.h"
using namespace std;
using namespace glm;
class Model
{
public:
	static enum { OUTSIDE, INTERSECT, INSIDE };
	Model(const string& objFilepath, const string& matFilepath, GLuint shaderProgram_, RendererType rendererType_);
	~Model();
	void Render(Camera *camera_, string tag);
	void AddMesh(OpenGLMesh* mesh_);
	int CreateInstance(vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	void UpdateInstance(int index_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	BoundingBox GetBoundingBox();
	GLuint getShaderProgram();
	mat4 GetTransform(int index_) const;
	int AABBandFrustum(Camera * camera);

private:
	std::vector<OpenGLMesh*> subMesh;
	GLuint shaderProgram;

	glm::mat4 GetTransform(vec3 position_, float angle_, vec3 rotation_, vec3 scale_) const;
	void LoadModel();
	vector<mat4> modelInstances;
	LoadOBJModel* obj;
	BoundingBox box;
	RendererType rendererType;
};
#endif // !MODEL_H
