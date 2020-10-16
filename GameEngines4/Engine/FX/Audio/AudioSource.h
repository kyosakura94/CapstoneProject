#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <string>
#include <glm/glm.hpp>
#include "../../Rendering/3D/Component.h"

using namespace glm;
using namespace std;

class GameObject;
class AudioSource : public Component
{
public:

	AudioSource(string name_, bool isLoop_ = false, bool is3D_ = false, bool mode_ = false);
	virtual ~AudioSource();
	virtual bool OnCreate(GameObject* parent);
	virtual void Update(float deltaTime);
	int  playSound();
	bool isPlaying(int channelID_);

private:
	string name;
	vec3 position, velocity;
	float volume;
	int channelID;
	GameObject * gameObject;
};

#endif // !AUDIOSOURCE_H


