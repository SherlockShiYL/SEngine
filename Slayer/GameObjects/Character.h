#ifndef INCLUDED_SLAYER_CHARACTER_H
#define INCLUDED_SLAYER_CHARACTER_H

#include <AI/Inc/AI.h>
#include "../Command.h"

enum class BotDirection
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum class CharacterType
{
	Player,
	Slime,
};

class Character :public S::AI::Agent
{
public:
	//Character();
	~Character();

	// Has pointer, not copyable
	Character(const Character&) = delete;
	Character& operator=(const Character&) = delete;

	Character(S::AI::AIWorld& world, int type, uint32_t id);

	bool IsActive() const							{ return mActive; }
	void Wake()										{ mActive = true; }
	void Kill()										{ mActive = false; }

	void SetBasicHP(int hp)							{ mBasicHP = hp; }
	void ResetHp()									{ mHP = mBasicHP; }
	int GetBasicHP() const							{ return mBasicHP; }
	void ReduceHP(int incomedamage);

	void Load(uint32_t id)							{ Id() = id; }
	void Update(float deltaTime)					{}

	void AddCommand(const Command& command);
	void CheckDirection(const Command& command);

	void SetPosition(S::Math::Vector2 p)			{ Position() = p; }
	const S::Math::Vector2 GetPosition() const		{ return Position(); }

	void SetSpeed(float s)							{ mSpeed = s; }
	void ResetSpeed()								{ mSpeed = mMaxSpeed; }
	void SetBasicSpeed(float s)						{ MaxSpeed() = s; }
	float GetBasicSpeed() const						{ return MaxSpeed(); }

	void SetDamage(int d)							{ mDamage = d; }
	void ResetDamage()								{ mDamage = mBasicDamage; }
	void SetBasicDamage(int d)						{ mBasicDamage = d; }
	int GetBasicDamage() const						{ return mBasicDamage; }

	void SetMoveCD(float m)							{ mMoveCD = m; }
	void ResetMoveCD()								{ mMoveCD = mBasicMoveCD; }
	void SetBasicMoveCD(float m)					{ mBasicMoveCD = m; }
	float GetBasicMoveCD() const					{ return mBasicMoveCD; }

	void SetMoveTime(float t)						{ mMoveTime = t; }
	void ResetMoveTime()							{ mMoveTime = mBasicMoveTime; }
	void SetBasicMoveTime(float t)					{ mBasicMoveTime = t; }
	float GetBasicMoveTime() const					{ return mBasicMoveTime; }

	void SetAttackSpeed(float attackspeed)			{ mAttackSpeed = attackspeed; }
	void ResetAttackSpeed()							{ mAttackSpeed = mBasicAttackSpeed; }
	void SetBasicAttackSpeed(float attackspeed)		{ mBasicAttackSpeed = attackspeed; }
	float GetBasicAttackSpeed() const				{ return mBasicAttackSpeed; }

	Command& GetCommand()							{ return mCommand; }
	std::list<Command>& GetCommandList()			{ return mCommandList; }

protected:
	bool mActive{ false };

	int mDamage{ 0 };
	int mBasicDamage{ 0 };
	int mHP{ 0 };
	int mBasicHP{ 0 };
	float mSpeed{ 0.0f };
	float mMoveCD{ 0.0f };
	float mBasicMoveCD{ 0.0f };
	float mMoveTime{ 0.0f };
	float mBasicMoveTime{ 0.0f };
	float mDyingTime{ 1.0f };
	float mAttackSpeed{ 1.0f };
	float mBasicAttackSpeed{ 1.0f }; // whole Attacking animation
	float mAttackAnimation{ 0.0f }; // when the attack working. gong ji qian yao

	std::list<Command> mCommandList;
	Command mCommand;
};

#endif // !INCLUDED_SLAYER_CHARACTER_H