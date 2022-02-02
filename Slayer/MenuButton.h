#ifndef INCLUDED_SLAYER_MENUBUTTON_H
#define INCLUDED_SLAYER_MENUBUTTON_H

#include <GameEngine/Inc/GameEngine.h>

struct MenuButton
{
	MenuButton(S::Geometry::Rect rect, S::Math::Vector2 p, const char* texture)
		: mRect{ rect }, mPosition{ p }
	{
		mId = S::Graphics::LoadTexture(texture);
	}

	void Pressed()								{ mPressed = true; }
	const bool IsPressed() const				{ return mPressed; }
	const S::Geometry::Rect GetRect(S::Math::Vector2 p) const
	{
		return S::Geometry::Rect{ mRect.left + p.x + mPosition.x
			,mRect.top + p.y + mPosition.y
			,mRect.right + p.x + mPosition.x
			,mRect.bottom + p.y + mPosition.y };
	}

	void Render(S::Math::Vector2 position)
	{
		S::Graphics::DrawScreenCircle({ position + mPosition,5.0f }, S::Math::Vector4::Orange());
		S::Graphics::DrawScreenRect(GetRect(position), S::Math::Vector4::Orange());
		S::Graphics::DrawSprite(mId, position + mPosition);
	}

	S::Geometry::Rect mRect;
	S::Math::Vector2 mPosition;
	S::Graphics::TextureId mId;
	bool mPressed{ false };
	bool mEnable{ false };
};

#endif // !INCLUDED_SLAYER_MENUBUTTON_H