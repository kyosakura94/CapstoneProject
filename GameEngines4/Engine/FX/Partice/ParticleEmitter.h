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
	ParticleEmitter(int numberParticle_, string shaderProgram_, string texture_);

	virtual ~ParticleEmitter();
	virtual void Update(const float deltaTime);
	virtual bool OnCreate(GameObject* parent_);
	virtual bool delayedUpdate() { return delayUpdate; };
	void Init(int numberParticle_, string shaderName_);
	void Render(Camera* camera_);

private:
	GameObject* gameObject;
	GLuint shaderProgram;
	Randomizer* random;
	std::vector<Particle*> particleList;
	bool delayUpdate;
};
#endif // !PARTICLEEMITTER_H



