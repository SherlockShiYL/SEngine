#include "Enemy.h"

using namespace S;

class IdleState;
class WalkState;
class AttackState;
class JumpState;
class SkillState;
class UltState;
class UseItemState;

//Enemy::Enemy(AI::AIWorld& world, uint32_t type, uint32_t id)
//	:Character{ world, type, id }
//{
//	MaxSpeed() = 200.0f;
//
//	//mStates = CharacterStates::Idle;
//
//	mStateMachine = new AI::StateMachine<Enemy>(*this);
//	mStateMachine->AddState<IdleState>();
//	mStateMachine->AddState<AttackState>();
//	mStateMachine->AddState<JumpState>();
//	mStateMachine->AddState<SkillState>();
//	mStateMachine->AddState<UltState>();
//	mStateMachine->AddState<UseItemState>();
//
//	//mStateMachine->ChangeState(static_cast<int>(CharacterStates::Idle));
//}

void Enemy::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);

}

void Enemy::AddCommand(const Command& command)
{
	Character::AddCommand(command);
}

void Enemy::CheckState()
{
	// Deserialize command and check if state is able to apply
	// Deserialize command and check if state is able to apply
	//if (!mCommandList.empty())
	//{
	//	for (auto& c : mCommandList)
	//	{
	//		switch (CharacterStates(mStates)) // How about switch(mStates)?
	//		{
	//		case CharacterStates::Idle:
	//			if (c.MoveDown() | c.MoveLeft() | c.MoveRight() | c.MoveUp())
	//			{
	//				mStates = CharacterStates::Walk;
	//				CheckDirection(c);
	//				mStateMachine->ChangeState(static_cast<uint32_t>(CharacterStates::Walk));
	//			}


	//			/*if (c.UseUlt())
	//			{
	//				mStates = CharacterStates::Ult;
	//				mStateMachine->ChangeState(CharacterStates::Ult);
	//			}*/

	//			break;
	//		case CharacterStates::Walk:
	//			if (!(c.MoveDown() | c.MoveLeft() | c.MoveRight() | c.MoveUp()))
	//			{
	//				mStates = CharacterStates::Idle;
	//				//CheckDirection(c);
	//				mStateMachine->ChangeState(static_cast<uint32_t>(CharacterStates::Idle));
	//			}
	//			break;
	//		case CharacterStates::Attack:
	//			break;
	//		case CharacterStates::Jump:
	//			break;
	//		case CharacterStates::Skill:
	//			break;
	//		case CharacterStates::Ult:
	//			break;
	//		case CharacterStates::UseItem:
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}
}

void Enemy::CheckDirection(const Command &command)
{
	Character::CheckDirection(command);
}

void Enemy::Render(Math::Vector2 p)
{
	Graphics::DrawScreenRect(mRect, Math::Vector4::Magenta());
}

void Enemy::PrintInfo(Math::Vector2 p)
{
	printf("%f,%f", Position().x, Position().y);
	//X::DrawScreenCircle({ Position() + p,10.0f }, X::Math::Vector4::Cyan());
}

// ====================== Class ======================
class ::IdleState :public AI::State<Enemy>
{
public:
	float timer{ 1.0f };
	float tileSize{ 0.0f };
	void Enter(Enemy& agent) override
	{
		//tileSize = agent.World().GetNavGraph().GetTileSize();
		//timer = 1.0f;
	}
	void Update(Enemy& agent, float deltaTime) override
	{
		// Should not be checkstate, should be read the command and apply here

	}
	void Exit(Enemy& agent) override {}
};

class ::WalkState :public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{
		agent.Position() += agent.Heading()*agent.MaxSpeed();
	}
	void Exit(Enemy& agent) override {}
};

class ::AttackState :public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override {}
};

class ::JumpState :public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override {}
};

class ::SkillState :public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override {}
};

class ::UltState :public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override {}
};

class ::UseItemState :public AI::State<Enemy>
{
public:
	void Enter(Enemy& agent) override
	{

	}
	void Update(Enemy& agent, float deltaTime) override
	{

	}
	void Exit(Enemy& agent) override {}
};