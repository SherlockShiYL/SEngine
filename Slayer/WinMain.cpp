#include "GameLevels/Frontend.h"
#include "GameLevels/Gameplay.h"
#include "GameLevels/EditMap.h"
#include "MenuManager.h"

GameState* currentState = nullptr;
Transition nextState = Transition::None;
const char* ipAdd{ nullptr };

void Initialize();
void Terminate();

bool GameLoop(float deltaTime)
{
	//auto start = std::chrono::system_clock::now();

	nextState = currentState->Update(deltaTime);

	switch (nextState)
	{
	case Transition::None:
	{
		currentState->Render();
		break;
	}
	case Transition::GoToFrontend:
	{
		currentState->Unload();
		delete currentState;
		currentState = new Frontend();
		currentState->Load();
		break;
	}
	case Transition::GoToPlay:
	{
		currentState->Unload();
		delete currentState;
		currentState = new Gameplay();
		currentState->Load();
		break;
	}
	case Transition::GoToDebug:
	{
		currentState->Unload();
		delete currentState;
		currentState = new EditMap();
		currentState->Load();
		break;
	}
	/*case Transition::GoToBattle:
	{
		currentState->Unload();
		delete currentState;
		currentState = new Gameplay(ipAdd);
		currentState->Load();
		break;
	}*/
	}

	/*auto end = std::chrono::system_clock::now();
	float desired = 1.0f / 60.0f * 1000.0f;
	float waitTime = desired - (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	if (waitTime > 0.0f)
	{
		Sleep(static_cast<DWORD>(waitTime));
	}*/

	return nextState == Transition::QuitGame;
}

void Initialize()
{
	currentState = new Frontend();
	currentState->Load();
}

void Terminate()
{
	currentState->Unload();
	S::SafeDelete(currentState);
	S::Graphics::ClearAllTextures();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	S::GameApplication myApp;

	myApp.Initialize(hInstance, "sconfig.json");

	Initialize();

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	Terminate();

	myApp.Terminate();

	return 0;
}