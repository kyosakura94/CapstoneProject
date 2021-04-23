#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../Engine/Core/CoreEngine.h"
#include <glm/gtx/string_cast.hpp>
#include "../../Game/Player/TestClassA.h"
#include "../../Engine/Rendering/2D/GuiImageComponent.h"
#include "../../Engine/Math/Quaternion.h"
#include "../../Engine/FX/Audio/AudioHandler.h"
#include "../../Engine/FX/Audio/AudioSource.h"
#include "../../Engine/Math/AI/Graph.h"
#include "../../Engine/Math/AI/node.h"
#include "../../Engine/Math/AI/AStarPathFinding.h"
#include "../../Engine/FX/Partice/ParticleEmitter.h"
#include "../../Engine/Math/Physics/GJKCollision.h"
#include "../../Engine/Math/Physics/CollisionResponse.h"
#include "../../Engine/Math/AI/StateMachine.h"
#include "../../Engine/Animation/animatedModel/AnimatedModel.h"
#include "../../Engine/Animation/animation_sub/Animator.h"
#include "../Player/PlayerMovement.h"
#include "../Player/Player.h"
#include "../Gun/Shoot.h"

//GET THE SHADER MAP
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void Draw();
	virtual void CreatePlayer(vec3 pos, string modelName, string tag);
	virtual void CreatePlayer(string json_);

	void createShadowMap();
	//SET UP SCENE
	void setUpPlayer();
	void setUpEnvironment();
	void setUpUI();
	void setUpSound();

private:
	unsigned int depthMapFBO;
	unsigned int depthMap;
	GameObject* player;
	SDL_Event e_;
	Graph<Node>  grid;
	AudioSource* backroundSound;
	ParticleEmitter* emmitter;
	float deltaTime;
	GJKCollision* GJKCheck;
	bool check;
	float infoDistance;
	Player* playerComponent;

};
#endif