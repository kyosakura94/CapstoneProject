#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;
class Joint
{
public:
	string name;
	int id;
	std::vector<Joint> children;

	mat4 animatedTransform;
	mat4 offset;
	mat4 localBindTransform;
	mat4 inverseBindTransform;

	Joint();
	Joint(int index_, string name_, mat4 bindLocalTransform_);
	~Joint();

	void addChild(Joint child);
	mat4 getanimatedTransform() { return animatedTransform; }
	void setanimatedTransform(mat4 transform) { animatedTransform = transform; }
	mat4 getinverseBindTransform() { return inverseBindTransform; }
	void calcInsverseBindTransform(mat4 parentBindTransform);

private:

	


};

