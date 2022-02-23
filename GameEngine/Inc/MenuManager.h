#ifndef INCLUDED_GAMEENGINE_MENUMANAGER_H
#define INCLUDED_GAMEENGINE_MENUMANAGER_H

#include "Menu.h"

namespace S {

class MenuManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static MenuManager* Get();

public:
	void Insert(Menu& menu);
	//void AddNewMenu(float width, float height, Math::Vector2 position, AnchorRow ar, AnchorColumn ac);
	//Menu GetMenu() { return mStartMenu; }
	void Render();
	void Update(float deltaTime);
	void Clear();

	bool Pressed(const char* buttonName, Input::MouseButton mouseButton);

	bool IsMouseCollided() const;

private:
	void CheckMouseIntersect();

	std::vector<Menu> mMenuList;
	bool isMouseCollided{ false };
	int currentMenuIndex{ -1 };

};

} // namesapce S


#endif // !INCLUDED_GAMEENGINE_MENUMANAGER_H
