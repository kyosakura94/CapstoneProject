#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(float lengthInSeconds, vector<KeyFrame> frames)
{
	length = lengthInSeconds;
	keyFrames = frames;
}

Animation::~Animation()
{
}

//Animation_B::Animation_B()
//{
//}
//
//Animation_B::~Animation_B()
//{
//}
