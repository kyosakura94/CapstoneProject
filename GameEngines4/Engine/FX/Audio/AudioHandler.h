#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <memory>
#include <fmod.hpp>

#include "../../Core/Debug.h"
#include "AudioSource.h"

using namespace glm;
using namespace std;

class AudioHandler 
{
	friend class AudioSource;
public:

	AudioHandler(const AudioHandler&) = delete;
	AudioHandler(AudioHandler&&) = delete;
	AudioHandler& operator = (const AudioHandler&) = delete;
	AudioHandler& operator = (AudioHandler&&) = delete;
	static AudioHandler* getInstance();

	bool Initialize(vec3 position_, vec3 velocity_, vec3 forwardDirection_, vec3 upDirection_);
	void OnDestroy();
	void Update();

private:

	AudioHandler();
	~AudioHandler();


	static std::unique_ptr<AudioHandler> audioInstance;
	friend std::default_delete<AudioHandler>;

	static map<string, FMOD::Sound*> soundObjects;
	static map<int, FMOD::Channel*> soundChannels;

	int channelCount;
	FMOD::System* systemobject = 0;
	vec3 position, velocity, forwardDirection, upDirection;
	FMOD_VECTOR convertvec3toFVector(vec3 vector_);
	void LoadSound(string name_, bool isLoop_, bool is3D_, bool mode_);

	FMOD::Sound * getSound(string name_);
	int  playSound(string name_, vec3 position_, vec3 velocity_ = vec3(0), float volume = 1.0f);
	int  stopSound(string name_);
	void UpdateVelocityAndPossition(int channelID_, vec3 position_, vec3 vel_ = vec3(0));
	bool isPlaying(int channelID_);
	bool isLoop, is3D, playmode;
};


#endif 


