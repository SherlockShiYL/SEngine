#include "GameApp.h"

bool GameLoop(float deltaTime)
{
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameApp myApp;
	myApp.Initialize(hInstance, "Ray Tracing!");

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	myApp.Terminate();
	return 0;
}