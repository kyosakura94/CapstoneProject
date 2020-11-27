#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <string>

using namespace glm;
using namespace std;
class Scene
{
public:
	Scene(){}
	virtual ~Scene(){}

	virtual bool OnCreate()= 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render() = 0;
	virtual void Draw() = 0;
	virtual void CreatePlayer(vec3 pos, string modelName, string tag) = 0;
	virtual void CreatePlayer(string json_) = 0;
private:

};
#endif // !SCENE_H
