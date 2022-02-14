#ifndef INCLUDED_GAMEENGINE_MENU_H
#define INCLUDED_GAMEENGINE_MENU_H

#include "MenuButton.h"

namespace S {

class Menu
{
public:
	Menu() = default;
	Menu(Geometry::Rect screenRect, float width, float height, Math::Vector2 position, AnchorRow ar, AnchorColumn ac);
	void Load(const Math::Vector2 position);
	void AddButton(float width, float height, const Math::Vector2 position,
		AnchorRow ar, AnchorColumn ac, const char* texture, std::string str);

	bool ButtonCheck(uint32_t i) const;
	void Update(const float deltaTime);
	void Render();

	bool CheckIntersect();

private:
	bool isActive{ false };
	bool isCollided{ false };
	AnchorRow mAnchorRow;
	AnchorColumn mAnchorColumn;
	Geometry::Rect mMenuRect;
	std::vector<MenuButton> mButtons;
	Math::Vector2 mPosition;
};

} // namespace S

#endif // !INCLUDED_GAMEENGINE_MENU_H
