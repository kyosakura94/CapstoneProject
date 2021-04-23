#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


#include "BaseMesh.h"

class OpenGLMesh : public BaseMesh
{
public:
	OpenGLMesh(SubMesh subMesh_, GLuint shaderProgram_);
	virtual ~OpenGLMesh();
	virtual void Render(Camera* camera_, std::vector<glm::mat4> instances);
	virtual void Render(Camera* camera_, std::vector<glm::mat4> instances, GLuint shadowMap_, mat4 lightLoc_);
	virtual void ShadowRender(Camera* camera_, std::vector<glm::mat4> instances);
	virtual void AnimatedRender(Camera* camera_, std::vector<glm::mat4> instances, int jointCout, std::vector<glm::mat4> currentpose);
	virtual void AnimatedRender(Camera* camera_, std::vector<glm::mat4> instances, int jointCout, std::vector<glm::mat4> currentpose, GLuint shadowMap_, mat4 lightLoc_);
	SubMesh getSubmesh();
	void useProgram(GLuint program_);
	void shadowGenerateBuffers(GLuint program_);
	void setLigtMatrix(glm::mat4 lightMatrix_) { lightmatrix = lightMatrix_; }
	void setDepthMap(unsigned int depthMap_) { depthMap = depthMap_; }
	void GenerateBuffers();
private:
	//void GenerateBuffers();
	SubMesh subMesh;
	GLuint VAO, VBO;
	GLuint shaderProgram;
	GLuint modelLoc, viewLoc, projLoc, bonLoc;
	GLuint viewPosition, lightPosition, ambientValue, diffuseValue, lightColour;
	//GLuint textureLoc;
	GLuint diffuseMap, shininess, transparency, ambient, diffuse, specular;
	GLuint depthMap, lightLoc;
	glm::mat4 lightmatrix;
};

#endif // !MESH_H



