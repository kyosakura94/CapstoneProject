#include "Engine/Core/CoreEngine.h"
#include "Game/GameTest.h"

int main(int argc, char* argv[]) 
{
	CoreEngine::GetInstance()->SetGameInterface(new GameTest);

	if (!CoreEngine::GetInstance()->OnCreate("Last Ancient Tower", 1080, 720))
	{
		return 0;
	}

	//Core game loop
	CoreEngine::GetInstance()->Run();

	printf("\n");

	return 0;
}

