#include "Precompiled.h"
#include "GameApplication.h"
#include "MenuManager.h"

#include <External/ImGui/Inc/imgui.h>

using namespace S;
using namespace S::Graphics;

GameApplication::GameApplication()
{}

GameApplication::~GameApplication()
{}

void GameApplication::OnInitialize()
{
	Config::StaticInitialize(Application::GetConfigFileName());

	HINSTANCE instance = GetModuleHandleA(nullptr);

	mWindow.Initialize(GetInstance(), Config::Get()->GetString("AppName", "S"),
		Config::Get()->GetInt("WinWidth", 1280), Config::Get()->GetInt("WinHeight", 720));
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), Config::Get()->GetBool("FullScreen", false));
	SimpleDraw::Initialize(1024 * 1024);
	Gui::Initialize(mWindow.GetWindowHandle());
	SpriteRenderer::StaticInitialize();
	TextureManager::StaticInitialize(Config::Get()->GetString("TexturePath", "../Assets/Images"));
	Physics::ColliderManager::StaticInitialize();
	MenuManager::StaticInitialize();

	timer.Initialize();

	mCamera.SetFOV(60.0f * Math::kDegToRad);
	mCamera.SetNearPlane(0.01f);
	mCamera.SetFarPlane(10000.0f);

	Font::StaticInitialize();
}

void GameApplication::OnTerminate()
{
	MenuManager::StaticTerminate();
	Physics::ColliderManager::StaticTerminate();
	Font::StaticTerminate();
	TextureManager::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	Gui::Terminate();
	SimpleDraw::Terminate();
	GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApplication::OnUpdate(bool(*GameLoop)(float))
{
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}
	/*if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}*/

	// deltaTime
	/*static auto prevTime = std::chrono::high_resolution_clock::now();
	auto currTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count() / 1000.0f;
	prevTime = currTime;*/

	// Update input and timer
	Input::InputSystem::Get()->Update();
	timer.Update();

	const float kDeltaTime = timer.GetElapsedTime();
	Gui::BeginRender();
	//ImGui::Text("FPS: %f", 1.0f / kDeltaTime);

	if (GameLoop(kDeltaTime))
	{
		PostQuitMessage(0);
		Gui::EndRender();
		return;
	}

	GraphicsSystem::Get()->BeginRender();

	// Sprites & Text
	Graphics::Render();

	SimpleDraw::Render(mCamera);
	MenuManager::Get()->Render();

	//ImGui::SetNextWindowPos({ 30.0f,30.0f });
	//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	Gui::EndRender();

	GraphicsSystem::Get()->EndRender();
}