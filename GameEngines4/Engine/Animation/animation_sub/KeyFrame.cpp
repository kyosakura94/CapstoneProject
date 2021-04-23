#include "KeyFrame.h"


KeyFrame::KeyFrame()
{
}

KeyFrame::KeyFrame(float timeStamp_, unordered_map<string, JointTransform> jointKeyFrames)
{
	timeStamp = timeStamp_;
	pose = jointKeyFrames;
}

KeyFrame::~KeyFrame()
{
}
