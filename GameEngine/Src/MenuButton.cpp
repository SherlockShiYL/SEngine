#include "Precompiled.h"
#include "MenuButton.h"

using namespace S;

MenuButton::MenuButton(Geometry::Rect menuRect, float width, float height, Math::Vector2 position, AnchorRow ar, AnchorColumn ac, const char* texture, std::string str)
	: mString{ str }, mAnchorRow{ ar }, mAnchorColumn{ ac }
{
	mId = Graphics::LoadTexture(texture);
	switch (ar)
	{
	case AnchorRow::Top:
		mPosition.y = menuRect.top + position.y;
		break;
	case AnchorRow::Center:
		mPosition.y = (menuRect.bottom - menuRect.top) * 0.5f + position.y;
		break;
	case AnchorRow::Bottom:
		mPosition.y = menuRect.bottom + position.y;
		break;
	default:
		break;
	}
	switch (ac)
	{
	case AnchorColumn::Left:
		mPosition.x = menuRect.left + position.x;
		break;
	case AnchorColumn::Center:
		mPosition.x = (menuRect.right - menuRect.left) * 0.5f + position.x;
		break;
	case AnchorColumn::Right:
		mPosition.x = menuRect.right + position.x;
		break;
	default:
		break;
	}
	mButtonRect = Geometry::Rect{ mPosition,width,height };
}

const Geometry::Rect MenuButton::GetRect()
{
	return mButtonRect;
}

void MenuButton::Render(Math::Vector2 position)
{
	Graphics::DrawScreenCircle({ position + mPosition,5.0f }, Math::Vector4::Orange());
	Graphics::DrawScreenRect(GetRect(), Math::Vector4::Orange());
	Graphics::DrawSprite(mId, position + mPosition);
}