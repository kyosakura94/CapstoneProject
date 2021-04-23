#include "AudioHandler.h"
#include "AudioSource.h"


AudioSource::AudioSource()
{
	channelID = -1;
}

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

bool AudioSource::delayedUpdate()
{
	return false;
}

void AudioSource::Init(string name_, bool isLoop_, bool is3D_, bool mode_)
{

	AudioHandler::getInstance()->LoadSound(name_, isLoop_, is3D_, mode_);
}

int AudioSource::playSound()
{
	if (gameObject != nullptr)
	{
		channelID = AudioHandler::getInstance()->playSound(name, gameObject->GetPosition(), vec3(0), volume);
	}
	else
	{
		channelID = AudioHandler::getInstance()->playSound(name, vec3(0), vec3(0), volume);
	}

	return channelID;
}

int AudioSource::playSound(string name_)
{
	if (gameObject != nullptr)
	{
		channelID = AudioHandler::getInstance()->playSound(name_, gameObject->GetPosition(), vec3(0), volume);
	}
	else
	{
		channelID = AudioHandler::getInstance()->playSound(name_, vec3(0), vec3(0), volume);
	}

	return channelID;
}


bool AudioSource::isPlaying(int channelID_)
{
	return AudioHandler::getInstance()->isPlaying(channelID_);
}


