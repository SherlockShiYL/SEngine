#include "Precompiled.h"
#include "Menu.h"

using namespace S;

Menu::Menu(Geometry::Rect screenRect, float width, float height, Math::Vector2 position, AnchorRow ar, AnchorColumn ac)
{
	mPosition = position;
	switch (ar)
	{
	case AnchorRow::Top:
		mPosition.y = screenRect.top + position.y;
		break;
	case AnchorRow::Center:
		mPosition.y = (screenRect.bottom - screenRect.top) * 0.5f + screenRect.top + position.y;
		break;
	case AnchorRow::Bottom:
		mPosition.y = screenRect.bottom + position.y;
		break;
	default:
		break;
	}
	switch (ac)
	{
	case AnchorColumn::Left:
		mPosition.x = screenRect.left + position.x;
		break;
	case AnchorColumn::Center:
		mPosition.x = (screenRect.right - screenRect.left) * 0.5f + screenRect.left + position.x;
		break;
	case AnchorColumn::Right:
		mPosition.x = screenRect.right + position.x;
		break;
	default:
		break;
	}
	mAnchorColumn = ac;
	mAnchorRow = ar;
	mMenuRect = Geometry::Rect{ mPosition,width,height };
}

void Menu::Load(const Math::Vector2 position)
{
	mPosition = position;
}

void Menu::AddButton(float width, float height, const Math::Vector2 position, AnchorRow ar, AnchorColumn ac, const char* texture, std::string str)
{
	mButtons.emplace_back(MenuButton(mMenuRect.right - mMenuRect.left, mMenuRect.bottom - mMenuRect.top, width, height, position, ar, ac, texture, str));
}

void Menu::SetButtonListDisplay(Math::Vector2 startPosition, AnchorRow ar, AnchorColumn ac, MenuDisplayOrder mode, int maxColumn, int maxRow, float horizontalGap, float verticalGap)
{
	switch (ar)
	{
	case AnchorRow::Top:
		break;
	case AnchorRow::Center:
		startPosition.y = (mMenuRect.bottom - mMenuRect.top) * 0.5f + startPosition.y;
		break;
	case AnchorRow::Bottom:
		startPosition.y = mMenuRect.bottom - mMenuRect.top + startPosition.y;
		break;
	default:
		break;
	}
	switch (ac)
	{
	case AnchorColumn::Left:
		break;
	case AnchorColumn::Center:
		startPosition.x = (mMenuRect.right - mMenuRect.left) * 0.5f + startPosition.x;
		break;
	case AnchorColumn::Right:
		startPosition.x = mMenuRect.right - mMenuRect.left + startPosition.x;
		break;
	default:
		break;
	}

	switch (mode)
	{
	case MenuDisplayOrder::Right:
		for (size_t i = 0; i < mButtons.size(); i++)
		{
			mButtons[i].SetPosition(startPosition + horizontalGap * (float)i);
		}
		break;
	case MenuDisplayOrder::Left:
		for (size_t i = 0; i < mButtons.size(); i++)
		{
			mButtons[i].SetPosition(startPosition - horizontalGap * (float)i);
		}
		break;
	case MenuDisplayOrder::Up:
		for (size_t i = 0; i < mButtons.size(); i++)
		{
			mButtons[i].SetPosition(startPosition - verticalGap * (float)i);
		}
		break;
	case MenuDisplayOrder::Down:
		for (size_t i = 0; i < mButtons.size(); i++)
		{
			mButtons[i].SetPosition(startPosition + verticalGap * (float)i);
		}
		break;
	case MenuDisplayOrder::RightDown:
		SetButtonsPosHorVert(startPosition, maxColumn, 1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::RightUp:
		SetButtonsPosHorVert(startPosition, maxColumn, 1, -1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::LeftDown:
		SetButtonsPosHorVert(startPosition, maxColumn, -1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::LeftUp:
		SetButtonsPosHorVert(startPosition, maxColumn, -1, -1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::UpLeft:
		SetButtonsPosVertHor(startPosition, maxRow, -1, -1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::UpRight:
		SetButtonsPosVertHor(startPosition, maxRow, 1, -1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::DownLeft:
		SetButtonsPosVertHor(startPosition, maxRow, -1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::DownRight:
		SetButtonsPosVertHor(startPosition, maxRow, 1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterRightDown:
		SetButtonsPosCenterHorVert(startPosition, maxColumn, 1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterLeftDown:
		SetButtonsPosCenterHorVert(startPosition, maxColumn, -1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterRightUp:
		SetButtonsPosCenterHorVert(startPosition, maxColumn, 1, -1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterLeftUp:
		SetButtonsPosCenterHorVert(startPosition, maxColumn, -1, -1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterDownLeft:
		SetButtonsPosCenterVertHor(startPosition, maxRow, -1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterUpLeft:
		SetButtonsPosCenterVertHor(startPosition, maxRow, -1, -1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterDownRight:
		SetButtonsPosCenterVertHor(startPosition, maxRow, 1, 1, horizontalGap, verticalGap);
		break;
	case MenuDisplayOrder::CenterUpRight:
		SetButtonsPosCenterVertHor(startPosition, maxRow, 1, -1, horizontalGap, verticalGap);
		break;
	default:
		break;
	}
}

int Menu::GetIntersectButtonIndex()
{
	Math::Vector2 mouseScreenPos = { (float)Input::InputSystem::Get()->GetMouseScreenX(),(float)Input::InputSystem::Get()->GetMouseScreenY() };
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		if (Geometry::PointInRect(mouseScreenPos, mButtons[i].GetRectInWorld({ mMenuRect.left,mMenuRect.top })))
		{
			return (int)i;
		}
	}
	return -1;
}

void Menu::CheckButtonsStatus(bool& isCollided)
{
	Math::Vector2 mouseScreenPos = { (float)Input::InputSystem::Get()->GetMouseScreenX(),(float)Input::InputSystem::Get()->GetMouseScreenY() };
	if (currentButtonIndex != -1)
	{
		if (!Geometry::PointInRect(mouseScreenPos, mButtons[currentButtonIndex].GetRectInWorld({ mMenuRect.left,mMenuRect.top })))
		{
			prevButtonIndex = currentButtonIndex;
			currentButtonIndex = -1;
			mButtons[prevButtonIndex].SetCollided(false);
		}
	}
	if (currentButtonIndex == -1)
		currentButtonIndex = GetIntersectButtonIndex();

	if (currentButtonIndex != -1)
	{
		isMouseCollided = true;
		isCollided = true;
		mButtons[currentButtonIndex].SetCollided(true);
		prevButtonIndex = currentButtonIndex;
		if (Input::InputSystem::Get()->IsMousePressed(Input::MouseButton::LBUTTON))
		{
			mButtons[currentButtonIndex].SetPressed(true);
		}
	}
	else
	{
		CheckMenuStatus(isCollided);
	}
}

void Menu::CheckMenuStatus(bool& isCollided)
{
	Math::Vector2 mouseScreenPos = { (float)Input::InputSystem::Get()->GetMouseScreenX(),(float)Input::InputSystem::Get()->GetMouseScreenY() };
	if (isCollidable && Geometry::PointInRect(mouseScreenPos, mMenuRect))
	{
		isMouseCollided = true;
		isCollided = true;
	}
	else
	{
		isMouseCollided = false;
		isCollided = false;
	}
}

void Menu::Update(const float deltaTime)
{

}

void Menu::Render()
{
	if (!isMouseCollided)
	{
		Graphics::DrawScreenCircle({ mPosition,5.0f }, Math::Vector4::Red());
		Graphics::DrawScreenRect(mMenuRect, Math::Vector4::Red());
	}
	else
	{
		Graphics::DrawScreenCircle({ mPosition,5.0f }, Math::Vector4::White());
		Graphics::DrawScreenRect(mMenuRect, Math::Vector4::White());
	}
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		mButtons[i].Render({ mMenuRect.left,mMenuRect.top });
	}
}

bool Menu::GetCollidable() const
{
	return isCollidable;
}

Geometry::Rect Menu::GetRect() const
{
	return mMenuRect;
}

void Menu::SetButtonsPosHorVert(Math::Vector2 startPosition, int maxColumn, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap)
{
	size_t columnIndex = 0;
	size_t rowIndex = 0;
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		mButtons[i].SetPosition({ startPosition.x + horizontalGap * (float)columnIndex * horizontalDir, startPosition.y + verticalGap * (float)rowIndex * verticalDir });
		if (++columnIndex >= maxColumn)
		{
			columnIndex = 0;
			++rowIndex;
		}
	}
}

void Menu::SetButtonsPosVertHor(Math::Vector2 startPosition, int maxRow, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap)
{
	size_t columnIndex = 0;
	size_t rowIndex = 0;
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		mButtons[i].SetPosition({ startPosition.x + horizontalGap * (float)columnIndex * horizontalDir, startPosition.y + verticalGap * (float)rowIndex * verticalDir });
		if (++rowIndex >= maxRow)
		{
			rowIndex = 0;
			++columnIndex;
		}
	}
}

void Menu::SetButtonsPosCenterHor(Math::Vector2 startPosition, size_t maxColumn, size_t amountInRow, size_t rowIndex, int horizontalDir, float horizontalGap)
{
	bool isOdd = maxColumn % 2;
	float columnIndex = isOdd ? (float)-((int)maxColumn >> 1) : (float)-((int)maxColumn >> 1) + 0.5f;
	for (size_t i = 0; i < amountInRow; i++)
	{
		mButtons[i + rowIndex * maxColumn].SetPosition({ startPosition.x + columnIndex * horizontalGap * horizontalDir,startPosition.y });
		++columnIndex;
	}
}

void Menu::SetButtonsPosCenterHorVert(Math::Vector2 startPosition, size_t maxColumn, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap)
{
	size_t maxRow = mButtons.size() / maxColumn;
	for (size_t i = 0; i < maxRow; i++)
	{
		SetButtonsPosCenterHor(startPosition, maxColumn, maxColumn, i, horizontalDir, horizontalGap);
		startPosition.y += verticalDir * verticalGap;
	}
	size_t lastRowAmount = mButtons.size() % maxColumn;
	SetButtonsPosCenterHor(startPosition, maxColumn, lastRowAmount, maxRow, horizontalDir, horizontalGap);
}

void Menu::SetButtonsPosCenterVer(Math::Vector2 startPosition, size_t maxRow, size_t amountInColumn, size_t columnIndex, int verticalDir, float verticalGap)
{
	bool isOdd = amountInColumn % 2;
	float rowIndex = isOdd ? (float)-((int)amountInColumn >> 1) : (float)-((int)amountInColumn >> 1) + 0.5f;
	for (size_t i = 0; i < amountInColumn; i++)
	{
		mButtons[i + columnIndex * maxRow].SetPosition({ startPosition.x,startPosition.y + rowIndex * verticalGap * verticalDir });
		++rowIndex;
	}
}

void Menu::SetButtonsPosCenterVertHor(Math::Vector2 startPosition, size_t maxRow, int horizontalDir, int verticalDir, float horizontalGap, float verticalGap)
{
	size_t maxColumn = mButtons.size() / maxRow;
	for (size_t i = 0; i < maxColumn; i++)
	{
		SetButtonsPosCenterVer(startPosition, maxRow, maxRow, i, verticalDir, verticalGap);
		startPosition.x += horizontalDir * horizontalGap;
	}
	size_t lastColumnAmount = mButtons.size() % maxRow;
	SetButtonsPosCenterVer(startPosition, maxRow, lastColumnAmount, maxColumn, verticalDir, verticalGap);
}
