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
	void Initialize();
	void Render();
	void Update(float deltaTime);
	bool CheckIntersect();
	void Clear();

private:
	Geometry::Rect mScreenRect;
	std::vector<Menu> mMenuList;

};

} // namesapce S


#endif // !INCLUDED_GAMEENGINE_MENUMANAGER_H
