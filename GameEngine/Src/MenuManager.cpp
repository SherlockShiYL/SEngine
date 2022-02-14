#include "Precompiled.h"
#include "MenuManager.h"

using namespace S;

namespace
{
	MenuManager* sMenuManager = nullptr;
}

void MenuManager::StaticInitialize()
{
	ASSERT(sMenuManager == nullptr, "[GameEngine::MenuManager] Manager already initialized!");
	sMenuManager = new MenuManager();
	sMenuManager->Initialize();
}

void MenuManager::StaticTerminate()
{
	if (sMenuManager != nullptr)
	{
		SafeDelete(sMenuManager);
	}
}

MenuManager* MenuManager::Get()
{
	ASSERT(sMenuManager != nullptr, "[GameEngine::MenuManager] No Manager registered.");
	return sMenuManager;
}

void MenuManager::Insert(Menu& menu)
{
	mMenuList.push_back(menu);
}

void MenuManager::Initialize()
{
	mScreenRect = Geometry::Rect(0.0f, 0.0f, (float)Graphics::GetScreenWidth(), (float)Graphics::GetScreenHeight());
}

void MenuManager::Render()
{
	for (auto& menu : mMenuList)
	{
		
		menu.Render();
	}
}

void MenuManager::Update(float deltaTime)
{
	for (auto& menu : mMenuList)
	{
		menu.Update(deltaTime);
	}
}

bool MenuManager::CheckIntersect()
{
	for (size_t i = 0; i < mMenuList.size(); i++)
	{

	}
	return false;
}

void MenuManager::Clear()
{
	mMenuList.clear();
}
