#ifndef NETWORSCENE_H
#define NETWORSCENE_H

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
#include "../../Engine/Network/Client.h"
#include "../../Engine/Network/Server.h"
#include "../../Engine/Event/KeyEventListener.h"

#include <json.hpp>

using json = nlohmann::json;

class NetworkScene : public Scene
{
public:
	NetworkScene();
	virtual ~NetworkScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void Draw();

private:
	bool input[4];
};

#endif // !NETWORSCENE_H


