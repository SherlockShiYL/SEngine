#ifndef INCLUDED_SLAYER_PLAYER_H
#define INCLUDED_SLAYER_PLAYER_H

#include "Skills/AttackProjectile.h"
#include "AI/Character.h"
#include "Skills/SkillList.h"
#include "Weapon.h"

enum class PlayerStates
{
	Idle,
	Walk,
	Attack,
	Jump,
	Skill,
	Ult,
	UseItem
};

class Player :public Character
{
public:
	// Has pointer, not copyable
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	Player(S::AI::AIWorld& world, AttackProjectiles* attackProjectiles, int type, uint32_t id);

	void Wake()											{ Character::Wake(); }
	void Kill()											{ Character::Kill(); }
	const bool IsActive()								{ return Character::IsActive(); }

	void Update(float deltaTime);

	void AddCommand(const Command& command);
	void CheckState();
	void CheckDirection(const Command& command);

	void PrintInfo(S::Math::Vector2 p);

	uint32_t GetId() const								{ return Id(); }

	//void SetCastAttackProjectile(bool b)				{ mCastAttackProjectile = b; }
	//const bool CheckCastAttackProjectile() const		{ return mCastAttackProjectile; }

	void SetDamage(int d)								{ mDamage = d; }
	const int GetDamage() const							{ return mDamage; }

	void SetHP(int hp)									{ mHP = hp; }
	const int GetHP() const								{ return mHP; }

	void SetSpeed(float s)								{ mSpeed = s; }
	const float GetSpeed() const						{ return mSpeed; }

	void SetName(std::string name)						{ mName = name; }
	const std::string GetName()							{ return mName; }

	ActiveSkillList CastSkill(ActiveSkillList n)		{ mSkillNumber = n; }
	void CleanSkill()									{ mSkillNumber = ActiveSkillList::None; }

	const ActiveSkillLists GetSkillList() const			{ return mPassiveSkillNumber; }

	Weapon SetWeapon(Weapon *w) { mWeapon = w; }

	void LoadTexture();
	void Render(S::Math::Vector2 p, float scale);

	S::AI::StateMachine<Player>* GetStateMachine()		{ return mStateMachine; }
	void SetState(PlayerStates state)					{ mStates = state; }

	S::Geometry::Rect SpriteRect(uint32_t stateindex, uint32_t animationindex);

	class IdleState;
	class WalkState;
	class AttackState;
	class JumpState;
	class SkillState;
	class UltState;
	class UseItemState;

private:
	AttackProjectiles* mAttackProjectiles{ nullptr };

	S::AI::StateMachine<Player>* mStateMachine{ nullptr };
	PlayerStates mStates;

	bool mCastAttackProjectile{ false };

	ActiveSkillList mSkillNumber{ ActiveSkillList::None };
	ActiveSkillLists mPassiveSkillNumber;

	std::string mName;

	// Items' information
	Weapon *mWeapon;

	// Textures
	S::Graphics::TextureId mTextureId;
	uint32_t mTextureWidth{ 64 };
	uint32_t mTextureHeight{ 64 };
	int mStateIndex{ 0 };
	int mAnimationIndex{ 0 };

	float animationTime{ 0.0f };
};

#endif // !INCLUDED_SLAYER_PLAYER_H