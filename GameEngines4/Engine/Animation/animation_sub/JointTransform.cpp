#include "JointTransform.h"

JointTransform::JointTransform()
{
}

JointTransform::JointTransform(vec3 position_, quat rotation_)
{
	position = position_;
	rotation = rotation_;
}

JointTransform::~JointTransform()
{
}
