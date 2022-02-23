#ifndef INCLUDED_SLAYER_SLIME_H
#define INCLUDED_SLAYER_SLIME_H

#include "../AI/Character.h"
#include "../Skills/AttackProjectile.h"

class Slime :public Character
{
	// All enemy need to be single class
	// Server need lots of memory to store runtime data
	// No need for enmey class anymore
	// Two checks about active, first in Update(), second in check projectile

public:
	Slime(S::AI::AIWorld& aiworld, uint32_t type, uint32_t id, AttackProjectiles* projectiles);
	~Slime();

	// Has pointer, not copyable
	Slime(const Slime&) = delete;
	Slime& operator=(const Slime&) = delete;

	void CopyData(Slime& slime);

	void Wake()									{ Character::Wake(); }
	const bool IsActive()						{ return mActive; }

	void Update(float deltaTime);

	void CheckDirection(const Command& command);

	void Render(S::Math::Vector2 p, float scale);

	void PrintInfo(S::Math::Vector2 p);

	void SetName(std::string n)					{ mName = n; }
	void SetSpeed(float speed)					{ MaxSpeed() = speed; }

	void SetRadius(float r)						{ Radius() = r; }
	float GetRadius() const						{ return Radius(); }

	S::AI::StateMachine<Slime>* GetStateMachine()	{ return mStateMachine; }

	bool SearchPlayer();

	class IdleState;
	class Jumptate;
	class GetDamageState;

private:
	S::AI::StateMachine<Slime>* mStateMachine{ nullptr };

	S::AI::Agent* mTarget{ nullptr };
	AttackProjectiles* mProjectiles{ nullptr };
	std::string mName{ "Slime" };
	S::Graphics::TextureId mId;

	S::AI::SteeringModule* mSteeringModule{ nullptr };
	S::AI::SeparationBehavior* mSeparationBehavior{ nullptr };
	//S::Physics::CircleCollider* mCollider;
};

#endif // !INCLUDED_SLAYER_SLIME_H