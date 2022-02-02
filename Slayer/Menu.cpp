#include "Menu.h"

using namespace S;

void Menu::Load(const Math::Vector2 position)
{
	mPosition = position;
}

void Menu::AddButton(S::Geometry::Rect rect, const S::Math::Vector2 position, const char* texture)
{
	mButtons.emplace_back(rect, position, texture);
}

const bool Menu::ButtonCheck(uint32_t i) const
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
			Math::Vector2 temp = { static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX())
				, static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) };
			if (Geometry::PointInRect( { static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX())
				, static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) }
				, b.GetRect(mPosition)))
			{
				b.Pressed();
			}
		}
	}
}

void Menu::Render()
{
	S::Graphics::DrawScreenCircle({ mPosition,5.0f }, S::Math::Vector4::Red());

	for (auto& b : mButtons)
	{
		b.Render(mPosition);
	}
}
