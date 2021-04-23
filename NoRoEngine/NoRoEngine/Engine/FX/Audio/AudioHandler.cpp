#include "AudioHandler.h"

std::unique_ptr<AudioHandler> AudioHandler::audioInstance = nullptr;
map<string, FMOD::Sound*> AudioHandler::soundObjects = map<string, FMOD::Sound*>();
map<int, FMOD::Channel*> AudioHandler::soundChannels = map<int, FMOD::Channel*>();

AudioHandler::AudioHandler() 
{

}
AudioHandler::~AudioHandler()
{

}

FMOD_VECTOR AudioHandler::convertvec3toFVector(vec3 vector_)
{
	FMOD_VECTOR variable;
	variable.x = vector_.x;
	variable.y = vector_.y;
	variable.z = vector_.z;

	return variable;
}

void AudioHandler::LoadSound(string name_, bool isLoop_, bool is3D_, bool mode_)
{
	isLoop = isLoop_;
	is3D = is3D_;
	playmode = mode_;

	if (getSound(name_) != nullptr)
	{
		return;
	}

	FMOD_MODE mode = FMOD_DEFAULT;

	if (isLoop_ == true)
	{
		mode = mode | FMOD_LOOP_NORMAL;
	}
	else
	{
		mode = mode | FMOD_LOOP_OFF;
	}

	if (is3D_ == true)
	{
		mode = mode | FMOD_3D;
	}
	else
	{
		mode = mode | FMOD_2D;
	}

	if (playmode == true)
	{
		mode = mode | FMOD_CREATESTREAM;
	}
	else
	{
		mode = mode | FMOD_CREATECOMPRESSEDSAMPLE;
	}

	FMOD::Sound *sound = nullptr;
	
	std::string str = "./Resources/Sound/" + name_;
	const char* cstr = str.c_str();

	if (systemobject->createSound(cstr, mode, nullptr, &sound) == FMOD_OK)
	{
		soundObjects[name_] = sound;
	}
}

FMOD::Sound * AudioHandler::getSound(string name_)
{
	if (soundObjects.find(name_) != soundObjects.end())
	{
		return soundObjects[name_];
	}

	return nullptr;
}

int AudioHandler::playSound(string name_, vec3 position_, vec3 velocity_, float volume_)
{
	int channelID = -1;

	if (getSound(name_) == nullptr)
	{
		LoadSound(name_, isLoop, is3D, playmode);

		if (getSound(name_) == nullptr)
		{
			return channelID;
		}
	}

	FMOD::Channel* channel;

	if (systemobject->playSound(getSound(name_), nullptr, true, &channel) == FMOD_OK)
	{
		channel->set3DAttributes(&convertvec3toFVector(position_), &convertvec3toFVector(velocity_));
		channel->setVolume(volume_);
		channel->setPaused(false);

		channelID = channelCount;
		channelCount++;

		/*if (channel->getIndex(&channelID) == FMOD_OK)
		{

		}*/
	}

	soundChannels[channelID] = channel;

	return channelID;
}

int AudioHandler::stopSound(string name_)
{
	int channelID = -1;

	if (getSound(name_) == nullptr)
	{
		return channelID;
	}

	FMOD::Channel* channel;

	if (systemobject->playSound(getSound(name_), nullptr, true, &channel) == FMOD_OK)
	{
		channel->setPaused(true);

		channelID = channelCount;
		channelCount++;
	}

	soundChannels[channelID] = channel;

	return channelID;
}

void AudioHandler::UpdateVelocityAndPossition(int channelID_, vec3 position_, vec3 vel_)
{
	if (soundChannels[channelID_] != nullptr)
	{
		soundChannels[channelID_]->set3DAttributes(&convertvec3toFVector(position_), &convertvec3toFVector(vel_));
	}
}

bool AudioHandler::isPlaying(int channelID_)
{
	bool play = false;
	if (soundChannels[channelID_] != nullptr)
	{
		soundChannels[channelID_]->isPlaying(&play);
	}

	return play;
}



AudioHandler* AudioHandler::getInstance()
{
	if (audioInstance.get() == nullptr)
	{
		audioInstance.reset(new AudioHandler);
	}

	return audioInstance.get();
}


bool AudioHandler::Initialize(vec3 position_, vec3 velocity_, vec3 forwardDirection_, vec3 upDirection_)
{
	int driver = 0;

	if (FMOD::System_Create(&systemobject) != FMOD_OK)
	{
		return false;
	}

	if (systemobject->getNumDrivers(&driver) != FMOD_OK)
	{
		return false;
	}

	if (driver == 0)
	{
		return false;
	}

	if (systemobject->init(10, FMOD_INIT_NORMAL | FMOD_3D | FMOD_INIT_3D_RIGHTHANDED, nullptr) != FMOD_OK)
	{
		return false;
	}

	if (systemobject->set3DListenerAttributes(0, &convertvec3toFVector(position_), &convertvec3toFVector(velocity_), &convertvec3toFVector(forwardDirection_), &convertvec3toFVector(upDirection_)) != FMOD_OK)
	{
		return false;
	}

	return true;
}

void AudioHandler::OnDestroy()
{
	for (auto mat : soundObjects)
	{
		mat.second->release();
		mat.second = nullptr;
	}

	soundObjects.clear();

	for (auto mat : soundChannels)
	{
		mat.second->stop();
		mat.second = nullptr;
	}
	soundChannels.clear();

	systemobject->release();
	systemobject = nullptr;
}

void AudioHandler::Update()
{
	systemobject->update();
}