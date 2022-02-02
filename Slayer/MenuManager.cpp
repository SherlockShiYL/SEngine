#include "MenuManager.h"

void MenuManager::LoadStartMenu()
{
	
	mStartMenu.Load({ static_cast<float>(S::Graphics::GetScreenWidth() >> 1)
		, static_cast<float>(S::Graphics::GetScreenHeight() >> 1) });

	mStartMenu.AddButton({ -100.0f,-40.0f,100.0f,40.0f }, { 0.0f,-50.0f }, "");
	mStartMenu.AddButton({ -100.0f,-40.0f,100.0f,40.0f }, { 0.0f,50.0f }, "");
}

void MenuManager::Render()
{
	mStartMenu.Render();
}

void MenuManager::Update(float deltaTime)
{
	mStartMenu.Update(deltaTime);
}
