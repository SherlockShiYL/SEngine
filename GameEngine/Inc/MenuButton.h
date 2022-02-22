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
	MenuButton(float menuWidth, float menuHeight, float width, float height, Math::Vector2 position,
		AnchorRow ar, AnchorColumn ac, const char* texture, std::string str);

	bool IsPressed() const;
	bool Active() const;

	const Geometry::Rect GetRect();
	const Geometry::Rect GetRectInWorld(Math::Vector2 position);

	void Render(Math::Vector2 position = { 0.0f,0.0f });

protected:
	friend class Menu;
	void SetPosition(Math::Vector2 position);
	void SetPressed(bool pressed);
	void SetCollided(bool collided);

protected:
	bool isPressed{ false }; // May not use this
	bool isActive{ false };
	bool isMouseCollided{ false };
	Geometry::Rect mButtonRect;
	Math::Vector2 mPosition;
	Graphics::TextureId mId;
	std::string mString;
	AnchorRow mAnchorRow{ AnchorRow::Center };
	AnchorColumn mAnchorColumn{ AnchorColumn::Center };
};

} // namespace S

#endif // !INCLUDED_GAMEENGINE_MENUBUTTON_H