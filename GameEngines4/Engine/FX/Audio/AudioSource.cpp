#include "AudioHandler.h"
#include "AudioSource.h"


AudioSource::AudioSource(string name_, bool isLoop_, bool is3D_, bool mode_)
{
	channelID = -1;
	name = name_;
	AudioHandler::getInstance()->LoadSound(name_, isLoop_, is3D_, mode_);
}

AudioSource::~AudioSource()
{
}

bool AudioSource::OnCreate(GameObject* parent)
{
	gameObject = parent;

	return true;
}

void AudioSource::Update(float deltaTime)
{
}

int AudioSource::playSound()
{
	channelID = AudioHandler::getInstance()->playSound(name, vec3(0));
	return channelID;
}

bool AudioSource::isPlaying(int channelID_)
{
	return AudioHandler::getInstance()->isPlaying(channelID_);
}


