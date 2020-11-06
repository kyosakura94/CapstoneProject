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
	glm::vec2 texCoords;
	glm::vec3 colour;
};
struct SubMesh
{
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
};

#endif // !BASEMESH_H

