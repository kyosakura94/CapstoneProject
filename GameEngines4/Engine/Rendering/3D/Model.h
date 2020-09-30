#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "LoadOBJModel.h"
using namespace std;
using namespace glm;
class Model
{
public:
	static enum { OUTSIDE, INTERSECT, INSIDE };
	Model(const string& objFilepath, const string& matFilepath, GLuint shaderProgram_);
	~Model();
	void Render(Camera *camera_, string tag);
	void AddMesh(Mesh* mesh_);
	int CreateInstance(vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	void UpdateInstance(int index_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_);
	BoundingBox GetBoundingBox();
	GLuint getShaderProgram();
	mat4 GetTransform(int index_) const;
	int AABBandFrustum(Camera * camera);

private:
	std::vector<Mesh*> subMesh;
	GLuint shaderProgram;

	glm::mat4 GetTransform(vec3 position_, float angle_, vec3 rotation_, vec3 scale_) const;
	void LoadModel();
	vector<mat4> modelInstances;
	LoadOBJModel* obj;
	BoundingBox box;
};
#endif // !MODEL_H
