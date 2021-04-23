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

	//default
	Model();
	Model(const string& objFilepath, const string& matFilepath, GLuint shaderProgram_, RendererType rendererType_);

	virtual vector<mat4> getJointTransform() = 0;
	virtual int getJointCount() = 0;
	virtual void Test() = 0;
	virtual void update(float deltaTime_) = 0;
	virtual std::vector<glm::mat4> getCurrentPose() = 0;
	virtual Joint getRootJoint() = 0;
	virtual ~Model();
	virtual void setCurrentPose(std::vector<glm::mat4> getCurrentPose_) = 0;

	//virtual vector<mat4> getJointTransform();

	void Render(Camera* camera_, string tag);
	void Render(Camera* camera_, string tag, GLuint shadeUsing_, unsigned int depthMap, mat4 lightmatrix);
	void ShadowRender(Camera* camera_, GLuint shaderProgram_);
	void Render(Camera* camera_, GLuint shaderProgram_);
	void AddMesh(OpenGLMesh* mesh_);
	int CreateInstance(vec3 position_, float angle_, vec3 rotation_, vec3 scale_, quat rotatioQua_);
	void UpdateInstance(int index_, vec3 position_, float angle_, vec3 rotation_, vec3 scale_, quat rotatioQua_);
	void RemoveInstance(int index_);
	
	void ChangeProgram(GLuint program_);
	BoundingBox GetBoundingBox();

	GLuint getShaderProgram();
	mat4 GetTransform(int index_) const;
	int AABBandFrustum(Camera* camera);

	string getModelName() { return modelName; }
	void setModelName(string name_) { modelName = name_; }

	std::vector<OpenGLMesh*> getsubMesh() { return subMesh; };

	virtual LoadOBJModel* getLoadOBJModel() = 0;
	vector<vec3> getvertercies(int index_);
	virtual mat4 getglobalInverseTransform() = 0;
	int getVertercies() { return vertercies.size(); }
	vector<mat4> getmodelInstance() { return modelInstances; }

protected:

	void LoadModel();
	std::vector<OpenGLMesh*> subMesh;
	GLuint shaderProgram;
	glm::mat4 GetTransform(vec3 position_, float angle_, vec3 rotation_, vec3 scale_, quat rotationQua_) const;


	vector<mat4> modelInstances;
	vector<vec3> vertercies;
	vector<vec3> worldVertecies;
	LoadOBJModel* obj;
	BoundingBox box;
	RendererType rendererType;
	string modelName;


private:

};
#endif // !MODEL_H
