#ifndef MESH_H
#define MESH_H

#include "BaseMesh.h"


class OpenGLMesh : public BaseMesh
{
public:
	OpenGLMesh(SubMesh subMesh_, GLuint shaderProgram_);
	virtual ~OpenGLMesh();
	virtual void Render(Camera* camera_, std::vector<glm::mat4> instances);
	SubMesh getSubmesh();
private:
	void GenerateBuffers();
	SubMesh subMesh;
	GLuint VAO, VBO;
	GLuint shaderProgram;
	GLuint modelLoc, viewLoc, projLoc;
	GLuint viewPosition, lightPosition, ambientValue, diffuseValue, lightColour;
	//GLuint textureLoc;
	GLuint diffuseMap, shininess, transparency, ambient, diffuse, specular;

};

#endif // !MESH_H



