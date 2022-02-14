#ifndef INCLUDED_GAMEENGINE_MENUBUTTON_H
#define INCLUDED_GAMEENGINE_MENUBUTTON_H

namespace S {

enum class AnchorRow
{
	Top,
	Center,
	Bottom
};

enum class AnchorColumn
{
	Left,
	Center,
	Right
};

class MenuButton
{
public:
	MenuButton(Geometry::Rect menuRect, float width, float height, Math::Vector2 position,
		AnchorRow ar, AnchorColumn ac, const char* texture, std::string str);

	void Pressed()						{ mPressed = true; }
	bool IsPressed() const				{ return mPressed; }
	bool Active() const					{ return mActive; }

	const Geometry::Rect GetRect();

	void Render(Math::Vector2 position = { 0.0f,0.0f });

protected:
	bool mPressed{ false };
	bool mActive{ false };
	Geometry::Rect mButtonRect;
	Math::Vector2 mPosition;
	Graphics::TextureId mId;
	std::string mString;
	AnchorRow mAnchorRow{ AnchorRow::Center };
	AnchorColumn mAnchorColumn{ AnchorColumn::Center };
};

} // namespace S

#endif // !INCLUDED_GAMEENGINE_MENUBUTTON_H