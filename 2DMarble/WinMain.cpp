#include "ClientNetworkManager.h"

using namespace S;

bool Init(float deltaTime);
bool Play(float deltaTime);

std::function<bool(float)> Tick = Init;

const char* ipAdd{ nullptr };
Input::InputSystem* input{ nullptr };
//Network::SocketAddress mSocketAddress{ "127.0.0.1", 8888 };

bool Init(float deltaTime)
{
	input = Input::InputSystem::Get();
	Tick = Play;
	ClientNetworkManager::StaticInitialize();
	ClientNetworkManager::Get()->ConnectToServer(ipAdd, 8888);
	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

bool Play(float deltaTime)
{


	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

bool GameLoop(float deltaTime)
{
	return Tick(deltaTime);
}

void Terminate()
{
	ClientNetworkManager::StaticTerminate();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	S::GameApplication myApp;
	ipAdd = lpCmdLine;

	myApp.Initialize(hInstance, "2D Marble");

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	myApp.Terminate();
	return 0;
}