#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::ParticleEmitter(int numberParticle_, string shaderName_)
{
	particleList.reserve(numberParticle_);
	shaderProgram = ShaderHandler::getInstance()->GetShader(shaderName_);
	random = new Randomizer();
	delayUpdate = true;
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint guiProgram = ShaderHandler::getInstance()->GetShader("particleShader");
	glUseProgram(guiProgram);

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
			float y = random->rand(0.0f, 1.5f);
			float z = random->rand(0.0f, 2.5f);
			float size = random->rand(1.0f, 5.0f);


			particleList[i]->setOriginal(vec3(x, y, z) + gameObject->GetPosition());
			particleList[i]->setVelocity(gameObject->GetVelocity());
			particleList[i]->setColor(vec3(y,x,z));
			particleList[i]->setSize(size);

			particleList[i]->Update(timeDelta_);

			particleList[i]->Render(CoreEngine::GetInstance()->GetCamera());
		}
	}

	glDisable(GL_BLEND);
}

bool ParticleEmitter::OnCreate(GameObject* parent_)
{
	gameObject = parent_;
	gameObject->SetDelay(true);
	return true;
}

void ParticleEmitter::Init(int numberParticle_, string shaderName_)
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
		Particle* particle = new Particle(shaderProgram, pos);
		particleList.push_back(particle);
	}
}

void ParticleEmitter::Render(Camera * camera_)
{

}
