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

void MenuManager::Render()
{
	for (auto& menu : mMenuList)
	{
		
		menu.Render();
	}
}

void MenuManager::Update(float deltaTime)
{
	CheckMouseIntersect();
	for (auto& menu : mMenuList)
	{
		menu.Update(deltaTime);
	}
}

void MenuManager::CheckMouseIntersect()
{
	Math::Vector2 mouseScreenPos = { (float)Input::InputSystem::Get()->GetMouseScreenX(),(float)Input::InputSystem::Get()->GetMouseScreenY() };
	for (int i = (int)mMenuList.size() - 1; i >= 0; i--)
	{
		mMenuList[i].CheckButtonsStatus(isMouseCollided);
		if (isMouseCollided)
		{
			currentMenuIndex = i;
			return;
		}
		else
		{
			if (mMenuList[i].GetCollidable() && Geometry::PointInRect(mouseScreenPos, mMenuList[i].GetRect()))
			{
				isMouseCollided = true;
				currentMenuIndex = i;
				return;
			}
		}
	}
	currentMenuIndex = -1;
	isMouseCollided = false;
	return;
}

void MenuManager::Clear()
{
	mMenuList.clear();
}

bool MenuManager::Pressed(const char* buttonName, Input::MouseButton mouseButton)
{
	if (Input::InputSystem::Get()->IsMousePressed(mouseButton) && isMouseCollided)
	{
		if (mMenuList[currentMenuIndex].GetCurrentButtonName().compare(buttonName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool MenuManager::IsMouseCollided() const
{
	return isMouseCollided;
}
