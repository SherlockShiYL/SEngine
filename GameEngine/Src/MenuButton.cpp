#include "Precompiled.h"
#include "MenuButton.h"

using namespace S;

MenuButton::MenuButton(float menuWidth, float menuHeight, float width, float height, Math::Vector2 position, AnchorRow ar, AnchorColumn ac, const char* texture, std::string str)
	: mString{ str }, mAnchorRow{ ar }, mAnchorColumn{ ac }
{
	mId = Graphics::LoadTexture(texture);
	switch (ar)
	{
	case AnchorRow::Top:
		mPosition.y = position.y;
		break;
	case AnchorRow::Center:
		mPosition.y = menuHeight * 0.5f + position.y;
		break;
	case AnchorRow::Bottom:
		mPosition.y = menuHeight + position.y;
		break;
	default:
		break;
	}
	switch (ac)
	{
	case AnchorColumn::Left:
		mPosition.x = position.x;
		break;
	case AnchorColumn::Center:
		mPosition.x = menuWidth * 0.5f + position.x;
		break;
	case AnchorColumn::Right:
		mPosition.x = menuWidth + position.x;
		break;
	default:
		break;
	}
	mButtonRect = Geometry::Rect{ mPosition,width,height };
}

void MenuButton::SetPressed(bool pressed)
{
	isPressed = pressed;
}

void MenuButton::SetCollided(bool collided)
{
	isMouseCollided = collided;
}

bool MenuButton::IsPressed() const
{
	return isPressed;
}

bool MenuButton::Active() const
{
	return isActive;
}

const Geometry::Rect MenuButton::GetRect()
{
	return mButtonRect;
}

const Geometry::Rect MenuButton::GetRectInWorld(Math::Vector2 position)
{
	return mButtonRect + position;
}

void MenuButton::Render(Math::Vector2 position)
{
	if (!isMouseCollided)
	{
		Graphics::DrawScreenCircle({ mPosition + position,3.0f }, Math::Vector4::Orange());
		Graphics::DrawScreenRect(GetRect() + position, Math::Vector4::Orange());
		Graphics::DrawSprite(mId, mPosition + position);
		Graphics::DrawScreenText(mString.c_str(), mPosition + position, 10.0f, Math::Vector4::Orange());
	}
	else
	{
		Graphics::DrawScreenCircle({ mPosition + position,3.0f }, Math::Vector4::White());
		Graphics::DrawScreenRect(GetRect() + position, Math::Vector4::White());
		Graphics::DrawSprite(mId, mPosition + position);
		Graphics::DrawScreenText(mString.c_str(), mPosition + position, 10.0f, Math::Vector4::White());
	}
}

void MenuButton::SetPosition(Math::Vector2 position)
{
	mPosition = position;
	mButtonRect = Geometry::Rect{ mPosition,mButtonRect.right - mButtonRect.left,mButtonRect.bottom - mButtonRect.top };
}
