#ifndef INCLUDED_SLAER_WEAPON_H
#define INCLUDED_SLAER_WEAPON_H

#include <Geometry/Inc/Geometry.h>

class Weapon
{
public:
	const bool IsMelee() const { return mMelee; }
	const int GetDamage() const { return mDamage; }
	const uint32_t GetModelID() const { return mModelID; }
	const S::Geometry::Rect GetAttackRange() const { return mAttackRange; }
	const std::string GetName() const { return mName; }

private:
	bool mMelee{ false };
	int mDamage{ 0 };
	uint32_t mModelID{ 0 };
	S::Geometry::Rect mAttackRange;
	std::string mName;

};

#endif // !INCLUDED_SLAER_WEAPON_H