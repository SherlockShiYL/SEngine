#ifndef INCLUDED_GAMEENGINE_MENU_H
#define INCLUDED_GAMEENGINE_MENU_H

#include "MenuButton.h"

namespace S {

enum class MenuDisplayOrder
{
	Right,
	Left,
	Up,
	Down,
	RightDown,
	RightUp,
	LeftDown,
	LeftUp,
	UpLeft,
	UpRight,
	DownLeft,
	DownRight,
	CenterRightDown,
	CenterLeftDown,
	CenterRightUp,
	CenterLeftUp,
	CenterDownLeft,
	CenterUpLeft,
	CenterDownRight,
	CenterUpRight,
};

class Menu
{
public:
	Menu() = default;
	Menu(Geometry::Rect screenRect, float width, float height, Math::Vector2 position, AnchorRow ar, AnchorColumn ac);
	void Load(const Math::Vector2 position);
	void AddButton(float width, float height, const Math::Vector2 position,
		AnchorRow ar, AnchorColumn ac, const char* texture, std::string str);

	// startPosition(ButtonListStartPosition) applies to the menu's position, local position
	void SetButtonListDisplay(Math::Vector2 startPosition, AnchorRow ar, AnchorColumn ac, MenuDisplayOrder mode, int maxColumn, int maxRow, float horizontalGap, float verticalGap);

	void CheckButtonsStatus(bool& isMouseCollided);

	void Update(const float deltaTime);
	void Render();

	bool GetCollidable() const;
	Geometry::Rect GetRect() const;

private:
	void SetButtonsPosHorVert(Math::Vector2 startPosition, int maxColumn, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap);
	void SetButtonsPosVertHor(Math::Vector2 startPosition, int maxRow, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap);
	void SetButtonsPosCenterHor(Math::Vector2 startPosition, size_t maxColumn, size_t amountInRow, size_t rowIndex, int horizontalDir, float horizontalGap);
	void SetButtonsPosCenterHorVert(Math::Vector2 startPosition, size_t maxColumn, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap);
	void SetButtonsPosCenterVer(Math::Vector2 startPosition, size_t maxRow, size_t amountInColumn, size_t columnIndex, int verticalDir, float verticalGap);
	void SetButtonsPosCenterVertHor(Math::Vector2 startPosition, size_t maxRow, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap);
	int GetIntersectButtonIndex();
	void CheckMenuStatus(bool& isMouseCollided);

private:
	bool isActive{ false };
	bool isCollidable{ true };
	bool isMouseCollided{ false };
	int currentButtonIndex{ -1 };
	int prevButtonIndex{ -1 };
	AnchorRow mAnchorRow;
	AnchorColumn mAnchorColumn;
	Geometry::Rect mMenuRect;
	std::vector<MenuButton> mButtons;
	Math::Vector2 mPosition;
};

} // namespace S

#endif // !INCLUDED_GAMEENGINE_MENU_H
