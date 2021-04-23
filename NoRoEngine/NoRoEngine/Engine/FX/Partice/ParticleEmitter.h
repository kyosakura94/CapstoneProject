#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include <glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "../../Camera/Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "Particle.h"
#include "../../Core/CoreEngine.h"
#include "../../Rendering/3D/Component.h"

using namespace std;
using namespace glm;


class ParticleEmitter : public Component
{
public:
	ParticleEmitter();
	ParticleEmitter(int numberParticle_, string shaderName_ );
	ParticleEmitter(int numberParticle_, string shaderProgram_, float lifetime);

	virtual ~ParticleEmitter();
	virtual void Update(const float deltaTime);
	virtual bool OnCreate(GameObject* parent_);
	virtual bool delayedUpdate() { return delayUpdate; };
	bool Init(int numberParticle_, string shaderName_, float lifeTime_);
	void Render(Camera* camera_);
	void setColorRange(float x, float y, float z);
	void setSizeRange(float x, float y);
	void setLifetime(float lifeTime_);
	void setRandom(bool ran) { randomcheck = ran; };
	void setType(int type_) { type = type_; };

private:
	GameObject* gameObject;
	GLuint shaderProgram;
	Randomizer* random;
	std::vector<Particle*> particleList;
	void SmokeParticle(float time_);
	void TrailParticle(float time_);
	void TrailParticleVer(float time_);
	int numberVer;
	bool delayUpdate;
	float colorrangeX, colorrangeY, colorrangeZ;
	float sizerangeX, sizerangeY;
	float lifeTime;
	bool randomcheck = true;
	int type = 0;
	
};
#endif // !PARTICLEEMITTER_H



