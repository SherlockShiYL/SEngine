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
		mPosition.y = (screenRect.bottom - screenRect.top) * 0.5f + position.y;
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
		mPosition.x = (screenRect.right - screenRect.left) * 0.5f + position.x;
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
	mButtons.emplace_back(MenuButton(mMenuRect, width, height, position, ar, ac, texture, str));
}

bool Menu::ButtonCheck(uint32_t i) const
{
	ASSERT(i < mButtons.size(), "i out of mButtons' size.");
	return mButtons[i].IsPressed();
}

void Menu::Update(const float deltaTime)
{
	if (Input::InputSystem::Get()->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		for (auto& b : mButtons)
		{
			if (Geometry::PointInRect({ static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX())
				, static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) }
				, b.GetRect()))
			{
				b.Pressed();
			}
		}
	}
}

void Menu::Render()
{
	Graphics::DrawScreenCircle({ mPosition,5.0f }, Math::Vector4::Red());
	Graphics::DrawScreenRect(mMenuRect, Math::Vector4::Red());
	for (size_t i = 0; i < mButtons.size(); i++)
	{
		mButtons[i].Render();
	}
}

bool Menu::CheckIntersect()
{
	for (size_t i = 0; i < mButtons.size(); i++)
	{

	}
	return false;
}
