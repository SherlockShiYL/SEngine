#ifndef INCLUDING_SLAYER_TINYBOT_H
#define INCLUDING_SLAYER_TINYBOT_H

#include <AI/Inc/AI.h>
#include <GameEngine/Inc/GameEngine.h>

enum class BotDirection
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};
enum class BotStates
{
	Idle,
	Walk,
	Attack,
};

enum class ClassType
{
	Bot,
	Storage,
	Bed,
	Boiler,
	Tree,
};

class TinyBot : public S::AI::Agent
{
public:
	TinyBot(S::AI::AIWorld& world);

	// Has pointer, not copyable
	TinyBot(const TinyBot&) = delete;
	TinyBot& operator=(const TinyBot&) = delete;

	void Load(S::Math::Vector2 p, int type, int id = 0);

	void Update(float deltaTime);
	void Render(S::Math::Vector2 p) override;
	void DebugRender(S::Math::Vector2 p);

	bool SearchEntity(ClassType type, BotStates state);

	void SetDestination(int x, int y);
	void SetDestinationList();

	BotDirection GetDirection();

protected:
	friend class IdleState;
	friend class WalkState;
	friend class AttackState;

	S::AI::StateMachine<TinyBot>* mStateMachine;

	S::Graphics::TextureId mTextureId;
	S::Geometry::Rect mSprite[12]
	{ { 0.0f,0.0f,32.0f,36.0f }, // Up
	{ 32.0f,0.0f,64.0f,36.0f },
	{ 64.0f,0.0f,96.0f,36.0f },
	{ 0.0f,36.0f,32.0f,72.0f }, // Right
	{ 32.0f,36.0f,64.0f,72.0f },
	{ 64.0f,36.0f,96.0f,72.0f },
	{ 0.0f,72.0f,32.0f,108.0f }, // Down
	{ 32.0f,72.0f,64.0f,108.0f },
	{ 64.0f,72.0f,96.0f,108.0f },
	{ 0.0f,108.0f,32.0f,144.0f }, // Left
	{ 32.0f,108.0f,64.0f,144.0f },
	{ 64.0f,108.0f,96.0f,144.0f } };
	int mIndex{ 7 };
	int mTexCol{ 0 };
	BotDirection mDirection;
	BotStates mStates;
	std::vector<S::Math::Vector2> mDesList;
	float mAttackPeriod{ 1.0f };
	float mAttackAnimationRate{ 0.5f };

	bool isAttacking{ false };
};

#endif // !INCLUDING_SLAYER_TINYBOT_H