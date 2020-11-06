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

using namespace std;
using namespace glm;


class ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(int numberParticle_, string shaderName_ );
	ParticleEmitter(int numberParticle_, string shaderProgram_, string texture_);
	~ParticleEmitter();

	void Update(const float timeDelta_);
	void Render(Camera* camera_);

private:
	GLuint shaderProgram;
	Randomizer* random;
	std::vector<Particle*> particleList;
};
#endif // !PARTICLEEMITTER_H



