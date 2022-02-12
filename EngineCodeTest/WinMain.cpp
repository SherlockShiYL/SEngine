#include "GameState.h"
#include "Frontend.h"
#include "PathFinding.h"
#include "QuadtreeScene.h"

GameState* currentState = nullptr;
Transition nextState = Transition::None;

void Initialize();
void Terminate();

bool GameLoop(float deltaTime)
{
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
	case Transition::GoToPathFindingScene:
	{
		currentState->Unload();
		delete currentState;
		currentState = new PathFindingScene();
		currentState->Load();
		break;
	}
	case Transition::GoToQuadtree:
	{
		currentState->Unload();
		delete currentState;
		currentState = new QuadtreeScene();
		currentState->Load();
		break;
	}
	}

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


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
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