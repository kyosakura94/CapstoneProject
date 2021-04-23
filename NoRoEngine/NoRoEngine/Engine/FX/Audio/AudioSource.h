#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <string>
#include <glm/glm.hpp>
#include "../../Rendering/3D/Component.h"
#include "../../Rendering/3D/GameObject.h"

using namespace glm;
using namespace std;

class GameObject;
class AudioSource : public Component
{
public:
	AudioSource();
	AudioSource(string name_, bool isLoop_ = false, bool is3D_ = false, bool mode_ = false);
	virtual ~AudioSource();
	virtual bool OnCreate(GameObject* parent);
	virtual void Update(float deltaTime);
	virtual bool delayedUpdate();
	void Init(string name_, bool isLoop_ = false, bool is3D_ = false, bool mode_ = false);
	int  playSound();
	int  playSound(string name);
	bool isPlaying(int channelID_);
	void setVolume(float volume_) { volume = volume_; };

private:
	string name;
	vec3 position, velocity;
	float volume = 1.0f;
	int channelID;
	GameObject * gameObject;
};

#endif // !AUDIOSOURCE_H


