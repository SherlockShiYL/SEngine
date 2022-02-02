#include "Building.h"

using namespace S;

Building::Building(int hp, S::Math::Vector2 position, uint32_t level, int armor,
	float countdown, float incomeamount, S::Geometry::Rect rect)
	: mHP{ hp }, mPosition{ position }, mLevel{ level }, mArmor{ armor }
	, mIncomeCountDown{ countdown }, mIncomeAmount{ incomeamount }, mRect{ rect }
{
}

const Geometry::Rect Building::GetRect() const
{
	return { mRect.left + mPosition.x,mRect.top + mPosition.y,mRect.right
		+ mPosition.x,mRect.bottom + mPosition.y };
}
