#include "AttackProjectile.h"

using namespace S;

void AttackProjectile::Wake(int damage, uint32_t type, uint32_t id, float aliveTime, float speed, Math::Vector2 heading, Math::Vector2 position, ActiveSkillLists sl)
{
	mActive = true;
	mDamage = damage;
	mType = type;
	mId = id;
	mAliveTime = aliveTime;
	mSpeed = speed;
	mHeading = heading;
	mPosition = position;
	for (uint32_t i = 0; i < static_cast<uint32_t>(sl.size()); ++i)
	{
		mSkillList[i] = sl[i];
	}
}

void AttackProjectile::Update(float deltaTime)
{
	if (!mActive)
		return;
	if (mAliveTime < 0.0f)
	{
		mActive = false;
		return;
	}
	mAliveTime -= deltaTime;
	mPosition += mHeading * mSpeed * deltaTime;
}

void AttackProjectile::CastSkill()
{
	for (uint32_t i = 0; i < NumOfPassiveSkills; ++i)
	{
		switch (mSkillList[i])
		{
		case ActiveSkillList::None:
			break;
		case ActiveSkillList::ThunderFlash:
			break;
		case ActiveSkillList::BlackHole:
			break;
		case ActiveSkillList::ShivLighting:

			break;
		default:
			break;
		}
	}
}

void AttackProjectile::Render(Math::Vector2 position, float scale)
{
	Graphics::DrawScreenRect(GetRectInWorld(position, scale), Math::Vector4::Magenta());
}

// AttackProjectiles
void AttackProjectiles::Initialize()
{
	mAttackProjectiles.resize(MaxProjectile);
}

void AttackProjectiles::CreateProjectile(int damage, uint32_t type, uint32_t id, float aliveTime, float speed
	, S::Math::Vector2 heading, S::Math::Vector2 position, ActiveSkillLists sl)
{
	mAttackProjectiles[mCurrenIndex++].Wake(damage, type, id, aliveTime, speed, heading, position, sl);
	if (mCurrenIndex >= MaxProjectile)
		mCurrenIndex = 0u;
}

void AttackProjectiles::Update(float deltaTime)
{
	for (auto& p : mAttackProjectiles)
	{
		if (p.IsAlive())
			p.Update(deltaTime);
	}
}

void AttackProjectiles::Render(Math::Vector2 position, float scale)
{
	for (auto& p : mAttackProjectiles)
	{
		if (p.IsAlive())
			p.Render(position, scale);
	}
}
