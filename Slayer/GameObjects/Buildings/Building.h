#ifndef INCLUDED_SLAYER_BUILDING_H
#define INCLUDED_SLAYER_BUILDING_H

#include <GameEngine/Inc/GameEngine.h>

class Building
{
public:
	Building(int hp, S::Math::Vector2 position, uint32_t level, int armor,
		float countdown, float incomeamount, S::Geometry::Rect rect);

	virtual void Update(float deltaTime) {}
	virtual void Render(S::Math::Vector2 p, float scale) {}

	virtual void LoadTexture() = 0;
	virtual void Load(S::Math::Vector2 position) = 0;

	const S::Geometry::Rect GetRect() const;

protected:
	int mHP{ -1 };
	S::Math::Vector2 mPosition;
	uint32_t mLevel{ 0u };
	int mArmor{ -1 };
	float mIncomeCountDown{ 0.0f };
	float mIncomeAmount{ 0.0f };

	S::Graphics::TextureId mId;
	S::Geometry::Rect mRect{ 0.0f,0.0f,1.0f,1.0f };
};

#endif // !INCLUDED_SLAYER_BUILDING_H
