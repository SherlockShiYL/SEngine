#ifndef INCLUDED_SLAYER_MENUMANAGER_H
#define INCLUDED_SLAYER_MENUMANAGER_H

#include "Menu.h"

class MenuManager
{
public:
	void LoadStartMenu();

	Menu GetStartMenu() { return mStartMenu; }
	void Render();
	void Update(float deltaTime);


private:
	Menu mStartMenu;

};

#endif // !INCLUDED_SLAYER_MENUMANAGER_H