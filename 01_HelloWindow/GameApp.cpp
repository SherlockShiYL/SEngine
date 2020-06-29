#include "GameApp.h"

using namespace S;

GameApp::GameApp()
{}

GameApp::~GameApp()
{}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 1280, 720);
}

void GameApp::OnTerminate()
{
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}
}