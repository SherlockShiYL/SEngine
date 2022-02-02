#ifndef INCLUDED_SLAYER_ATTACKPROJECTILE_H
#define INCLUDED_SLAYER_ATTACKPROJECTILE_H

#include <GameEngine/Inc/GameEngine.h>
#include "SkillList.h"

#define NumOfPassiveSkills 10
#define MaxProjectile 100

struct AttackProjectile
{
public:
	void Wake(int damage, uint32_t type, uint32_t id
		, float aliveTime, float speed
		, S::Math::Vector2 heading, S::Math::Vector2 position
		, ActiveSkillLists sl);

	void Kill()									{ mActive = false; }

	void Update(float deltaTime);

	const int GetDamage() const					{ return mDamage; }

	const float GetAliveTime() const			{ return mAliveTime; }

	const bool IsAlive() const					{ return mActive; }

	const uint32_t GetType() const				{ return mType; }

	void CastSkill();

	const S::Geometry::Rect GetRectInWorld() const
	{
		return S::Geometry::Rect
		{ 
			mRect.left + mPosition.x,	mRect.top + mPosition.y,
			mRect.right + mPosition.x, mRect.bottom + mPosition.y 
		};
	}
	const S::Geometry::Rect GetRectInWorld(S::Math::Vector2 p, float scale) const
	{
		return S::Geometry::Rect
		{
			(mRect.left + mPosition.x)*scale + p.x,	(mRect.top + mPosition.y)*scale + p.y,
			(mRect.right + mPosition.x)*scale + p.x, (mRect.bottom + mPosition.y)*scale + p.y
		};
	}
	const S::Math::Vector2 GetPosition() const	{ return mPosition; }

	void Render(S::Math::Vector2 position, float scale);

private:
	bool mActive{ false };
	int mDamage{ 0 };
	uint32_t mType{ 0 }; // Enemy's 1 or Player's 0
	uint32_t mId{ 0 };
	float mAliveTime{ 0.0f };
	float mSpeed{ 0.0f };
	S::Math::Vector2 mHeading{ 0.0f,0.0f };
	S::Math::Vector2 mPosition{ 0.0f,0.0f };
	S::Geometry::Rect mRect{ -10.0f,-10.0f,10.0f,10.0f };

	ActiveSkillList mSkillList[NumOfPassiveSkills];
};

class AttackProjectiles
{
public:
	void Initialize();
	void CreateProjectile(int damage, uint32_t type, uint32_t id, float aliveTime, float speed
		, S::Math::Vector2 heading, S::Math::Vector2 position, ActiveSkillLists sl);

	void Update(float deltaTime);
	void Render(S::Math::Vector2 position, float scale);

	std::vector<AttackProjectile>& GetAttackProjectiles() { return mAttackProjectiles; }

private:
	std::vector<AttackProjectile> mAttackProjectiles;
	uint32_t mCurrenIndex{ 0u };
};

#endif // !INCLUDED_SLAYER_ATTACKPROJECTILE_H