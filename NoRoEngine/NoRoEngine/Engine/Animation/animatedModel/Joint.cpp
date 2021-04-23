#include "Joint.h"

Joint::Joint()
{
}

Joint::Joint(int index_, string name_, mat4 bindLocalTransform_)
{
	name = name_;
	id = index_;
	localBindTransform = bindLocalTransform_;
}

Joint::~Joint()
{
}

void Joint::addChild(Joint child)
{
	children.push_back(child);
}

void Joint::calcInsverseBindTransform(mat4 parentBindTransform)
{
	mat4 bindTransform = parentBindTransform * localBindTransform;
	inverseBindTransform =  inverse(bindTransform);

	for (Joint child : children)
	{
		child.calcInsverseBindTransform(bindTransform);
	}

}
