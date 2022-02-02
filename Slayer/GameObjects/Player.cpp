#include "Player.h"

using namespace S;

Player::Player(AI::AIWorld& world, AttackProjectiles* attackProjectiles, int type, uint32_t id)
	:Character{ world, type, id }
{
	mAttackProjectiles = attackProjectiles;
	MaxSpeed() = 200.0f;
	mPosition = { 100.0f,101.0f };
	mStates = PlayerStates::Idle;
	mRadius = 16.0f;
	mPassiveSkillNumber.push_back(mSkillNumber);
	mDamage = 5;

	mStateMachine = new AI::StateMachine<Player>(*this);
	mStateMachine->AddState<Player::IdleState>();
	mStateMachine->AddState<Player::WalkState>();
	mStateMachine->AddState<Player::AttackState>();
	mStateMachine->AddState<Player::JumpState>();
	mStateMachine->AddState<Player::SkillState>();
	mStateMachine->AddState<Player::UltState>();
	mStateMachine->AddState<Player::UseItemState>();

	mStateMachine->ChangeState(static_cast<int>(PlayerStates::Idle));
}

void Player::Update(float deltaTime)
{
	//CheckState();
	mStateMachine->Update(deltaTime);
	animationTime -= deltaTime;
	if (animationTime <= 0.0f)
	{
		animationTime = 0.3f;
		mAnimationIndex++;
		if (mAnimationIndex >= 3u)
		{
			mAnimationIndex = 0u;
		}
	}
	ImGui::SliderInt("State:", &mStateIndex, 0, 17);
	ImGui::SliderInt("Animation:", &mAnimationIndex, 0, 2);

}

void Player::AddCommand(const Command& command)
{
	Character::AddCommand(command);
}

void Player::CheckState()
{
	// Deserialize command and check if state is able to apply
	if (!mCommandList.empty())
	{
		for (auto& c : mCommandList)
		{
			switch (PlayerStates(mStates)) // How about switch(mStates)?
			{
			case PlayerStates::Idle:
				if (c.isMoveDown() | c.isMoveLeft() | c.isMoveRight() | c.isMoveUp())
				{
					mStates = PlayerStates::Walk;
					CheckDirection(c);
					mStateMachine->ChangeState(static_cast<int>(PlayerStates::Walk));
				}


				/*if (c.UseUlt())
				{
					mStates = PlayerStates::Ult;
					mStateMachine->ChangeState(PlayerStates::Ult);
				}*/

				break;
			case PlayerStates::Walk:
				if (!(c.isMoveDown() | c.isMoveLeft() | c.isMoveRight() | c.isMoveUp()))
				{
					mStates = PlayerStates::Idle;
					//CheckDirection(c);
					mStateMachine->ChangeState(static_cast<int>(PlayerStates::Idle));
				}
				break;
			case PlayerStates::Attack:
				break;
			case PlayerStates::Jump:
				break;
			case PlayerStates::Skill:
				break;
			case PlayerStates::Ult:
				break;
			case PlayerStates::UseItem:
				break;
			default:
				break;
			}
		}
	}
}

void Player::CheckDirection(const Command &command)
{
	Character::CheckDirection(command);
}

void Player::PrintInfo(Math::Vector2 p)
{
	printf("Player %d : Position %f,%f\n", GetId(), Position().x, Position().y);
	//S::DrawScreenCircle({ Position() + p,10.0f }, S::Math::Vector4::Cyan());
}



void Player::LoadTexture()
{
	//mTextureId = Graphics::LoadTexture("Bot/ninja_m.png"); // Load texture from texture pool
	mTextureId = Graphics::LoadTexture("Bot/SV_Girl.png"); // Load texture from texture pool
}

void Player::Render(Math::Vector2 p, float scale)
{
	Graphics::DrawScreenCircle({ {mPosition*scale + p},mRadius*scale }, Math::Vector4::Cyan());
	Graphics::DrawSprite(mTextureId, SpriteRect(mStateIndex, mAnimationIndex), (mPosition + Math::Vector2{ 256.0f, 160.0f }) *scale + p, 0.0f, scale);
}

Geometry::Rect Player::SpriteRect(uint32_t stateindex, uint32_t animationindex)
{
	uint32_t w = stateindex % 3;
	uint32_t h = stateindex / 3;
	Geometry::Rect mR{ static_cast<float>(mTextureWidth * animationindex + w * mTextureWidth * 3u)
							, static_cast<float>(h * mTextureHeight)
							, static_cast<float>(mTextureWidth * animationindex + w * mTextureWidth * 3u + mTextureWidth)
							, static_cast<float>(h * mTextureHeight + mTextureHeight) };
	return mR;
}

// ====================== Class ======================
class Player::IdleState :public AI::State<Player>
{
public:
	float timer{ 1.0f };
	float tileSize{ 0.0f };
	void Enter(Player& agent) override
	{
		//tileSize = agent.World().GetNavGraph().GetTileSize();
		//timer = 1.0f;
		agent.CheckDirection(agent.GetCommand());
	}
	void Update(Player& agent, float deltaTime) override
	{
		// Should not be checkstate, should be read the command and apply here
		// 2019.11.1 I moved the movement from here to WalkState
		if (agent.GetCommand().isAttack())
		{
			agent.SetState(PlayerStates::Attack);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Attack));
			return;
		}
		else if (agent.GetCommand().isJump())
		{
			agent.SetState(PlayerStates::Jump);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Jump));
			return;
		}
		else if (agent.GetCommand().isUseSkill())
		{
			agent.SetState(PlayerStates::Skill);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Skill));
			return;
		}
		else if (agent.GetCommand().isUseUlt())
		{
			agent.SetState(PlayerStates::Ult);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Ult));
			return;
		}
		else if (agent.GetCommand().isMoveDown() || agent.GetCommand().isMoveLeft() || agent.GetCommand().isMoveRight() || agent.GetCommand().isMoveUp())
		{
			agent.SetState(PlayerStates::Walk);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Walk));
			return;
		}
	}
	void Exit(Player& agent) override {}
};

class Player::WalkState :public AI::State<Player>
{
public:
	void Enter(Player& agent) override
	{

	}
	void Update(Player& agent, float deltaTime) override
	{
		if (agent.GetCommand().isAttack())
		{
			agent.SetState(PlayerStates::Attack);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Attack));
			return;
		}
		else if (!(agent.GetCommand().isMoveDown() || agent.GetCommand().isMoveLeft() || agent.GetCommand().isMoveRight() || agent.GetCommand().isMoveUp()))
		{
			agent.SetState(PlayerStates::Idle);
			//CheckDirection(c);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Idle));
			return;
		}

		if (agent.GetCommandList().empty())
		{
			agent.CheckDirection(agent.GetCommand());
			agent.Position() += agent.Heading()*agent.MaxSpeed()*deltaTime;
		}
		else
		{
			while (!agent.GetCommandList().empty())
			{
				//printf("Player->Update() Command: %d\n", agent.GetCommandList().front().mCommand);
				agent.CheckDirection(agent.GetCommandList().front());
				agent.Position() += agent.Heading()*agent.MaxSpeed()*deltaTime;
				//agent.mIndex = static_cast<int>(agent.mDirection)*agent.mTexCol + 1;

				agent.GetCommandList().pop_front();
			}
		}


	}
	void Exit(Player& agent) override {}
};

class Player::AttackState :public AI::State<Player>
{
public:
	float mTime{ 0.5f };
	void Enter(Player& agent) override
	{
		mTime = 0.5f;
		//agent.SetCastAttackProjectile(true);
	}
	void Update(Player& agent, float deltaTime) override
	{
		mTime -= deltaTime;



		if (mTime <= 0.0f)
		{
			agent.mAttackProjectiles->CreateProjectile(agent.mDamage, agent.mType, agent.mId, 3.0f, 500.0f,
				agent.mHeading, agent.mPosition + agent.mHeading * 10.0f, agent.mPassiveSkillNumber);


			agent.SetState(PlayerStates::Idle);
			agent.GetStateMachine()->ChangeState(static_cast<int>(PlayerStates::Idle));
			return;
		}
	}
	void Exit(Player& agent) override {}
};

class Player::JumpState :public AI::State<Player>
{
public:
	void Enter(Player& agent) override
	{

	}
	void Update(Player& agent, float deltaTime) override
	{

	}
	void Exit(Player& agent) override {}
};

class Player::SkillState :public AI::State<Player>
{
public:
	void Enter(Player& agent) override
	{

	}
	void Update(Player& agent, float deltaTime) override
	{

	}
	void Exit(Player& agent) override {}
};

class Player::UltState :public AI::State<Player>
{
public:
	void Enter(Player& agent) override
	{

	}
	void Update(Player& agent, float deltaTime) override
	{

	}
	void Exit(Player& agent) override {}
};

class Player::UseItemState :public AI::State<Player>
{
public:
	void Enter(Player& agent) override
	{

	}
	void Update(Player& agent, float deltaTime) override
	{

	}
	void Exit(Player& agent) override {}
};