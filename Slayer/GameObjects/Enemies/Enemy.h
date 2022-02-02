#ifndef INCLUDED_SLAYER_ENEMY_H
#define INCLUDED_SLAYER_ENEMY_H

#include "../AI/Character.h"

//=============================== DELETE THIS ==================================

class Enemy :public Character
{
public:
	~Enemy()									{ Character::~Character(); }

	// Has pointer, not copyable
	Enemy(const Enemy&) = delete;
	Enemy& operator=(const Enemy&) = delete;

	//Enemy(S::AI::AIWorld& world, uint32_t type, uint32_t id);

	void Wake()									{ Character::Wake(); }
	const bool IsActive()						{ return Character::IsActive(); }

	void Update(float deltaTime);

	void AddCommand(const Command& command);
	void CheckState();
	void CheckDirection(const Command& command);

	void Render(S::Math::Vector2 p);

	void PrintInfo(S::Math::Vector2 p);

	const S::Geometry::Rect GetRectInWorld() const
	{
		return { mRect.left + GetPosition().x,mRect.top + GetPosition().y,
		mRect.right + GetPosition().x,mRect.bottom + GetPosition().y };
	}

	void SetName(std::string n)					{ mName = n; }
	void SetSpeed(float speed)					{ MaxSpeed() = speed; }



private:
	S::AI::StateMachine<Enemy>* mStateMachine{ nullptr };
	//CharacterStates mStates;

	S::Geometry::Rect mRect{ 1.0f,1.0f,1.0f,1.0f };

	std::string mName;

	// Basic Ability
};

#endif // !INCLUDED_SLAYER_ENEMY_H