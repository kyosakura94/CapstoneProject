#include "Engine/Core/CoreEngine.h"
#include "Game/GameTest.h"

int main(int argc, char* argv[]) 
{
	CoreEngine::GetInstance()->SetGameInterface(new GameTest);
	if (!CoreEngine::GetInstance()->OnCreate("GameEngine", 800, 600))
	{
		return 0;
	}

	CoreEngine::GetInstance()->Run();

	return 0;
}