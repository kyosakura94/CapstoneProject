#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::ParticleEmitter(int numberParticle_, string shaderName_)
{
	particleList.reserve(numberParticle_);
	shaderProgram = ShaderHandler::getInstance()->GetShader(shaderName_);
	random = new Randomizer();

	for (size_t i = 0; i < numberParticle_; i++)
	{
		float x = random->rand(0.0f, 1.0f);
		float y = random->rand(0.0f, 1.0f);
		float z = random->rand(0.0f, 1.0f);

		vec3 pos = vec3(x, y, z);
		Particle * particle = new Particle(shaderProgram, pos);
		particleList.push_back(particle);
	}
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Update(const float timeDelta_)
{
	if (!particleList.empty()) 
	{
		for (int i = 0; i < particleList.size(); i++)
		{
			if (particleList[i]->getLifetime() <= 0)
			{
				particleList.erase(particleList.begin() + i);
				break;
			}
			float x = random->rand(0.0f, 1.0f);
			float y = random->rand(0.0f, 1.0f);
			float z = random->rand(0.0f, 1.0f);

			particleList[i]->setVelocity(vec3(x,y,z));
			particleList[i]->Update(timeDelta_);
		}
	}
}

void ParticleEmitter::Render(Camera * camera_)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint guiProgram = ShaderHandler::getInstance()->GetShader("particleShader");
	glUseProgram(guiProgram);

	for (auto par : particleList)
	{
		par->Render(camera_);
	}

	glDisable(GL_BLEND);
}
