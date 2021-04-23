#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
{
	delayUpdate = true;
}

ParticleEmitter::ParticleEmitter(int numberParticle_, string shaderName_)
{
	particleList.reserve(numberParticle_);
	shaderProgram = ShaderHandler::getInstance()->GetShader(shaderName_);
	random = new Randomizer();
	delayUpdate = true;
	colorrangeX = 0.0f;
	colorrangeY = 1.5f;
	sizerangeX = 10.0f;
	sizerangeY = 10.0f;
	lifeTime = 200.0f;
	vec3 poss;
	if (gameObject)
	{
		poss = gameObject->GetPosition();
	}
	else
	{
		poss = vec3(0, 0, 0);
	}
	
	for (size_t i = 0; i < numberParticle_; i++)
	{
		float x = random->rand(0.0f, 1.0f);
		float y = random->rand(0.0f, 1.0f);
		float z = random->rand(0.0f, 1.0f);

		vec3 pos = vec3(x, y, z);

		Particle * particle = new Particle(shaderProgram, pos + poss);
		particle->setLifeTime(lifeTime);
		particle->setSize(20.f);


		particleList.push_back(particle);
	}
}

ParticleEmitter::ParticleEmitter(int numberParticle_, string shaderName_, float lifetime)
{
	Init(numberParticle_, shaderName_, lifetime);
}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::Update(const float timeDelta_)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint guiProgram = ShaderHandler::getInstance()->GetShader("particleShader");
	glUseProgram(shaderProgram);
	switch (type)
	{
	case 0:
		SmokeParticle(timeDelta_);
		break;
	case 1:
		TrailParticle(timeDelta_);
		break;	
	case 2:
		TrailParticleVer(timeDelta_);
		break;
	default:
		break;
	}


	glDisable(GL_BLEND);
}

bool ParticleEmitter::OnCreate(GameObject* parent_)
{
	gameObject = parent_;
	gameObject->SetDelay(true);
	return true;
}

bool ParticleEmitter::Init(int numberParticle_, string shaderName_, float lifetime_)
{
	particleList.reserve(numberParticle_);
	shaderProgram = ShaderHandler::getInstance()->GetShader(shaderName_);
	delayUpdate = true;
	random = new Randomizer();
	colorrangeX = 0.0f;
	colorrangeY = 1.5f;
	colorrangeY = 1.5f;
	sizerangeX = 3.0f;
	sizerangeY = 10.0f;
	lifeTime = lifetime_;
	vec3 poss;
	if (gameObject)
	{
		poss = gameObject->GetPosition();
	}
	else
	{
		poss = vec3(0, 0, 0);
	}

	for (size_t i = 0; i < numberParticle_; i++)
	{
		float x = random->rand(0.0f, 1.0f);
		float y = random->rand(0.0f, 1.0f);
		float z = random->rand(0.0f, 1.0f);

		vec3 pos = vec3(x, y, z);

		Particle* particle = new Particle(shaderProgram, pos + poss );
		particle->setLifeTime(lifeTime);
		particleList.push_back(particle);
	}

	return true;
}

void ParticleEmitter::Render(Camera * camera_)
{

}

void ParticleEmitter::setSizeRange(float x, float y)
{
	sizerangeX = x;
	sizerangeY = y;
}

void ParticleEmitter::setLifetime(float lifeTime_)
{
	lifeTime = lifeTime_;
}

void ParticleEmitter::SmokeParticle(float timeDelta_)
{
	if (!particleList.empty())
	{
		for (int i = 0; i < particleList.size(); i++)
		{
			if (particleList[i]->getLifetime() < 0)
			{
				float x = random->rand(0.0f, 1.0f);
				float y = random->rand(0.0f, 1.0f);
				float z = random->rand(0.0f, 1.0f);

				particleList[i]->setOriginal(vec3(x, y, z) + gameObject->GetPosition());
				particleList[i]->setLifeTime(lifeTime);
				particleList[i]->setColor(vec3(random->rand(0.9f, 1.0f), random->rand(0.0f, 0.8f), 0));
				//particleList.erase(particleList.begin() + i);
				break;
			}
			float x, y, z;
			float size;
			if (randomcheck)
			{
				x = random->rand(colorrangeX, colorrangeX);
				y = random->rand(colorrangeY, colorrangeX);
				z = random->rand(0, 0);
				//size = random->rand(sizerangeX, sizerangeY);
			}
			else
			{
				x = colorrangeX;
				y = colorrangeY;
				z = colorrangeZ;
				size = sizerangeX;
			}

			particleList[i]->setVelocity(gameObject->GetVelocity());
			//particleList[i]->setColor(vec3(x, y, z));
			particleList[i]->setSize(10.f);

			particleList[i]->Update(timeDelta_);

			particleList[i]->Render(CoreEngine::GetInstance()->GetCamera());
		}
	}
}

void ParticleEmitter::TrailParticle(float timeDelta_)
{
	if (!particleList.empty())
	{
		for (int i = 0; i < particleList.size(); i++)
		{
			if (particleList[i]->getLifetime() < 0)
			{
				float x = random->rand(0.0f, 1.0f);
				float y = random->rand(0.0f, 1.0f);
				float z = random->rand(0.0f, 1.0f);

				particleList[i]->setOriginal(vec3(x, 0 , 0) + gameObject->GetPosition());
				particleList[i]->setLifeTime(lifeTime);
				particleList[i]->setColor(vec3(random->rand(0.9f, 1.0f), random->rand(0.0f, 0.8f), 0));
				//particleList.erase(particleList.begin() + i);
				break;
			}

			float x, y, z;
			float size;
			if (randomcheck)
			{
				x = random->rand(0.0f, 1.0f);
				y = random->rand(0.0f, 1.0f);
				z = random->rand(0.0f, 1.0f);
				size = random->rand(sizerangeX, sizerangeY);
			}
			else
			{
				x = colorrangeX;
				y = colorrangeY;
				z = colorrangeZ;
				size = sizerangeX;
			}

			particleList[i]->setOriginal(vec3(x, 0, 0) + gameObject->GetPosition());
			particleList[i]->setVelocity(gameObject->GetVelocity());
			particleList[i]->setColor(vec3(x, y, z));
			particleList[i]->setSize(10.f);

			particleList[i]->Update(timeDelta_);

			particleList[i]->Render(CoreEngine::GetInstance()->GetCamera());
		}
	}

}

void ParticleEmitter::TrailParticleVer(float timeDelta_)
{
	if (!particleList.empty())
	{
		for (int i = 0; i < particleList.size(); i++)
		{
			if (particleList[i]->getLifetime() < 0)
			{
				float x = random->rand(0.0f, 1.0f);
				float y = random->rand(0.0f, 1.0f);
				float z = random->rand(0.0f, 1.0f);

				particleList[i]->setOriginal(vec3(0, 0, z) + gameObject->GetPosition());
				particleList[i]->setLifeTime(lifeTime);
				particleList[i]->setColor(vec3(random->rand(0.9f, 1.0f), random->rand(0.0f, 0.8f), 0));
				//particleList.erase(particleList.begin() + i);
				break;
			}

			float x, y, z;
			float size;
			if (randomcheck)
			{
				x = random->rand(0.0f, 1.0f);
				y = random->rand(0.0f, 1.0f);
				z = random->rand(0.0f, 1.0f);
				size = random->rand(sizerangeX, sizerangeY);
			}
			else
			{
				x = colorrangeX;
				y = colorrangeY;
				z = colorrangeZ;
				size = sizerangeX;
			}

			particleList[i]->setOriginal(vec3(0, 0, z) + gameObject->GetPosition());
			particleList[i]->setVelocity(gameObject->GetVelocity());
			particleList[i]->setColor(vec3(x, y, z));
			particleList[i]->setSize(10.f);

			particleList[i]->Update(timeDelta_);

			particleList[i]->Render(CoreEngine::GetInstance()->GetCamera());
		}
	}

}

void ParticleEmitter::setColorRange(float x, float y, float z)
{
	colorrangeX = x;
	colorrangeY = y;
	colorrangeZ = z;
}
