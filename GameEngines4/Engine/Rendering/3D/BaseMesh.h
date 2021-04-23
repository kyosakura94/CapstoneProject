#ifndef BASEMESH_H
#define BASEMESH_H

#include <glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Camera/Camera.h"
#include "../../Graphics/MaterialHandler.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 colour;
	glm::vec4 boneIds = glm::vec4(0);
	glm::vec4 boneWeights = glm::vec4(0.0f);
};
struct SubMesh
{
	unsigned int BaseVertex; //!< Total number of mesh indices. 
	unsigned int BaseIndex; //!< The base vertex of this mesh in the vertices array for the entire model.
	unsigned int NumIndices; //!< The base index of this mesh in the indices array for the entire model. 
	unsigned int MaterialIndex;

	std::vector<Vertex> vertexList;
	std::vector<int> meshIndices;
	Material material;
};

class BaseMesh
{
public:
	BaseMesh() {}
	virtual ~BaseMesh() {}
	virtual void Render(Camera* camera_, std::vector<glm::mat4> instances) = 0;
	virtual void Render(Camera* camera_, std::vector<glm::mat4> instances, GLuint shadowMap_, mat4 lightLoc_) = 0;
	virtual void ShadowRender(Camera* camera_, std::vector<glm::mat4> instances) = 0;
	virtual void AnimatedRender(Camera* camera_, std::vector<glm::mat4> instances, int jointCout, std::vector<glm::mat4> currentpose) = 0;
};

#endif // !BASEMESH_H

