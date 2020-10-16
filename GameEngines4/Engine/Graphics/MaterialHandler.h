#ifndef MATERIALHANDELER_H
#define MATERIALHANDELER_H

#include <memory>
#include <glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

struct Material
{
	inline	Material() : diffuseMap(0), shininess(0.0f), transparency(0.0f), ambient(vec3(1.0f)) , diffuse(vec3(1.0f)) ,spectular(vec3())
	{
	
	}
	GLuint diffuseMap;
	float shininess;
	float transparency;

	vec3 ambient;
	vec3 diffuse;
	vec3 spectular;


	string name;
};

class MaterialHandler
{
public:
	MaterialHandler(const MaterialHandler&) = delete;
	MaterialHandler(MaterialHandler&&) = delete;
	MaterialHandler& operator = (const MaterialHandler&) = delete;
	MaterialHandler& operator = (MaterialHandler&&) = delete;

	static MaterialHandler * getInstance();

	void AddMaterial(Material mat_);
	const Material GetMaterial(const string & matName_);
	void OnDestroy();

private:
	MaterialHandler();
	~MaterialHandler();
	static unique_ptr<MaterialHandler> materialInstance;
	friend default_delete<MaterialHandler>;

	static vector<Material> materials;
};

#endif // !1

