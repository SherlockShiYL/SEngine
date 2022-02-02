#include "Precompiled.h"
#include "Application.h"

using namespace S;
using namespace S::Core;

Application::Application()
	: mInstance{ nullptr }
	, mWindow{ nullptr }
	, mRunning{ true }
{
}

Application::~Application()
{
}

void Application::Initialize(HINSTANCE instance, LPCSTR configFileName)
{
	mInstance = instance;
	mConfigFileName = configFileName;

	CoInitialize(nullptr);
	OnInitialize();
}

void Application::Terminate()
{
	OnTerminate();
	CoUninitialize();
}

void Application::HookWindow(HWND hWnd)
{
	mWindow = hWnd;
}

void Application::UnhookWindow()
{
	mWindow = nullptr;
}

void Application::Update(bool(*GameLoop)(float))
{
	OnUpdate(GameLoop);
}