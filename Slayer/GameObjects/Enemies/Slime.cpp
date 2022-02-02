#include "Slime.h"

using namespace S;
namespace
{
	enum BotState
	{
		Idle,
		Jump,
		Attack,
	};
}

Slime::Slime(AI::AIWorld& aiworld, uint32_t type, uint32_t id, AttackProjectiles* projectiles)
	: Character{ aiworld, type, id }
{
	mStateMachine = new AI::StateMachine<Slime>(*this);
	mStateMachine->AddState<Slime::IdleState>();
	mStateMachine->AddState<Slime::Jumptate>();
	mStateMachine->AddState<Slime::GetDamageState>();

	mStateMachine->ChangeState(static_cast<uint32_t>(BotState::Idle));
	mProjectiles = projectiles;

	mId = Graphics::LoadTexture("Bot/Skeleton.png");
}

void Slime::CopyData(Slime& slime)
{
	SetBasicHP(slime.GetBasicHP());
	SetBasicAttackSpeed(slime.GetBasicAttackSpeed());
	SetBasicDamage(slime.GetBasicDamage());
	SetBasicMoveCD(slime.GetBasicMoveCD());
	SetBasicMoveTime(slime.GetBasicMoveTime());
	SetBasicSpeed(slime.GetBasicSpeed());

	SetRadius(slime.Radius());
	ResetAttackSpeed();
	ResetDamage();
	ResetHp();
	ResetMoveCD();
	ResetMoveTime();
	ResetSpeed();

	//Physics::CircleCollider mCollider(&mPosition, mRadius);
	//Physics::AddCollider(mCollider, 0);

	mSteeringModule = new AI::SteeringModule(*this);
	mSeparationBehavior = mSteeringModule->AddBehavior<AI::SeparationBehavior>();
	mSeparationBehavior->SetActive(true);

}

void Slime::Update(float deltaTime)
{
	if (mHP <= 0)
		mActive = false;

	if (mCommand.isJump())
	{
		mCommand.Clear();
		mCommandList.clear();
		if (!SearchPlayer())
		{
			mStateMachine->ChangeState(static_cast<uint32_t>(BotState::Idle));
		}
	}
	if (mCommand.isAttack()) // Get Damaged
	{
		mCommand.Clear();
		mCommandList.clear();
		mStateMachine->ChangeState(static_cast<uint32_t>(BotState::Attack));
	}

	mStateMachine->Update(deltaTime);
}

void Slime::CheckDirection(const Command &command)
{
	Character::CheckDirection(command);
}

bool Slime::SearchPlayer()
{
	mTarget = World().GetOneAgent(static_cast<uint32_t>(CharacterType::Player));

	if (mTarget)
	{
		mDestination = mTarget->Position();
		mStateMachine->ChangeState(static_cast<uint32_t>(BotState::Jump));
		return true;
	}
	return false;
}

void Slime::Render(Math::Vector2 p, float scale)
{
	//Graphics::DrawScreenCircle({ mPosition * scale + p,mRadius * scale }, Math::Vector4::Magenta());
	char hp[6];
	_itoa_s(mHP, hp, 10);
	Graphics::DrawScreenText(hp, mPosition.x * scale + p.x, mPosition.y * scale + p.y, 24.0f * scale, Math::Vector4::Magenta());
	Graphics::DrawSprite(mId, { mPosition.x * scale + p.x, mPosition.y * scale + p.y }, 0.0f, scale);
}

void Slime::PrintInfo(Math::Vector2 p)
{
	printf("%f,%f", mPosition.x, mPosition.y);
	S::Graphics::DrawScreenCircle({ Position() + p,10.0f }, S::Math::Vector4::Cyan());
}

// ====================== Class ======================
class Slime::IdleState :public AI::State<Slime>
{
public:
	float timer{ 0.0f };
	void Enter(Slime& agent) override
	{
		timer = 0.0f;
	}
	void Update(Slime& agent, float deltaTime) override
	{
		// Should not be checkstate, should be read the command and apply here
		timer -= deltaTime;
		if (timer <= 0.0f)
		{
			agent.mCommand.Jump();
		}
	}
	void Exit(Slime& agent) override {}
};

class Slime::Jumptate :public AI::State<Slime>
{
public:
	int index{ 0 };
	int frame{ 1 };
	//float timer{ ANIMATIONTIME };
	float checker{ 0.5f };
	float timer{ 1.0f };
	void Enter(Slime& agent) override
	{
		agent.Velocity() = Math::Normalize(agent.Destination() - agent.mPosition);

		index = 0;
		frame = 1;
		//timer = ANIMATIONTIME;
		//checker = DESTINATIONCHECK;

		//agent.mDirection = agent.GetDirection();
		//agent.mIndex = static_cast<int>(agent.mDirection)*agent.mTexCol + 1;

		timer = 1.0f;
	}
	void Update(Slime& agent, float deltaTime) override
	{
		if (Math::DistanceSqr(agent.Position(), agent.mTarget->Position()) <=
			agent.Radius()*agent.Radius() + agent.mTarget->Radius()*agent.mTarget->Radius())
		{
			agent.GetStateMachine()->ChangeState(static_cast<uint32_t>(BotState::Attack));
			return;
		}
		timer -= deltaTime;


		if (timer >= 0.0f)
		{
			checker -= deltaTime;
			if (checker <= 0.0f)
			{
				agent.Heading() = Math::Normalize(agent.Destination() - agent.Position());
				agent.Velocity() = agent.Heading()*agent.MaxSpeed();
			}

			Math::Vector2 tempVelocity = agent.mSteeringModule->Calculate();
			if (Math::Dot(tempVelocity, agent.Velocity()) < 0.0)
			{
				agent.Velocity() += tempVelocity;
			}



			// WARNNING: Can NOT change the Destination during the Moving
			if (Math::DistanceSqr(agent.Position(), agent.Destination()) > agent.MaxSpeed()*0.1f)
			{
				agent.Position() += agent.Velocity()*deltaTime;
			}

			/*if (timer <= 0.0f)
			{
				timer = ANIMATIONTIME;
				++index;
				if (!static_cast<bool>(index % 2))
				{
					frame = -frame;
				}
				agent.mIndex += frame;
			}*/

			// Check Arrived and State
			if (Math::DistanceSqr(agent.Position(), agent.Destination()) <= agent.MaxSpeed()*0.1f)
			{
				agent.Position() = agent.Destination();
				agent.GetStateMachine()->ChangeState(static_cast<uint32_t>(BotState::Idle));
			}
		}
		else
		{
			agent.GetStateMachine()->ChangeState(static_cast<uint32_t>(BotState::Idle));
		}
	}
	void Exit(Slime& agent) override {}
};

class Slime::GetDamageState :public AI::State<Slime>
{
public:
	bool attacked{ false };
	void Enter(Slime& agent) override
	{
		agent.mAttackAnimation = 0.3f;
		attacked = false;
	}
	void Update(Slime& agent, float deltaTime) override
	{
		agent.mAttackAnimation -= deltaTime;
		agent.mAttackSpeed -= deltaTime;
		if (agent.mAttackAnimation <= 0.0f && !attacked)
		{
			ActiveSkillLists temp;
			temp.push_back(ActiveSkillList::AttackProjectile);
			agent.mProjectiles->CreateProjectile(10, 1, agent.Id(), 0.3f, 0.0f
				, { 0.0f,0.0f }, Math::Normalize(agent.Destination() - agent.Position())*16.0f + agent.Position(), temp);
			attacked = true;
		}
		if (agent.mAttackSpeed <= 0.0f)
		{
			agent.GetStateMachine()->ChangeState(static_cast<uint32_t>(BotState::Idle));
		}
	}
	void Exit(Slime& agent) override 
	{
		agent.mAttackSpeed = 1.0f;
	}
};
