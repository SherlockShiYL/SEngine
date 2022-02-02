#ifndef INCLUDED_SLAYER_MENU_H
#define INCLUDED_SLAYER_MENU_H

#include "MenuButton.h"

class Menu
{
public:
	void Load(const S::Math::Vector2 position);
	void AddButton(const S::Geometry::Rect rect, const S::Math::Vector2 position, const char* texture);

	const bool ButtonCheck(uint32_t i) const;
	void Update(const float deltaTime);
	void Render();

private:
	std::vector<MenuButton> mButtons;
	S::Math::Vector2 mPosition;
	bool isActive{ false };

};

#endif // !INCLUDED_SLAYER_MENU_H
