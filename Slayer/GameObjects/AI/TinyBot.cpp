#include "TinyBot.h"

using namespace S;

#define ANIMATIONTIME 0.25f
#define DESTINATIONCHECK 0.5f

class IdleState;
class WalkState;
class AttackState;

TinyBot::TinyBot(AI::AIWorld& world)
	:Agent{ world }
{}

void TinyBot::Load(Math::Vector2 p, int type, int id)
{
	Entity::Load(p, type, id);
	mTextureId = Graphics::LoadTexture("Bot/ninja_m.png");

	mPosition = { 304.0f,304.0f };
	mDestination = mPosition;
	mVelocity = { 0.0f,0.0f };
	mMaxSpeed = 64.0f;
	mTexCol = 3;
	mStates = BotStates::Idle;

	mStateMachine = new AI::StateMachine<TinyBot>(*this);
	mStateMachine->AddState<IdleState>();
	mStateMachine->AddState<WalkState>();
	mStateMachine->AddState<AttackState>();


	mStateMachine->ChangeState(static_cast<int>(BotStates::Idle));
}

void TinyBot::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
}

void TinyBot::Render(Math::Vector2 p)
{
	Graphics::DrawSprite(mTextureId, mSprite[mIndex], mPosition + p + Math::Vector2{ 32.0f, 54.0f });
	if (mDesList.size() > 0)
	{
		for (uint32_t i = 0; i < mDesList.size() - 1; ++i)
		{
			Graphics::DrawScreenLine(mDesList[i]+p, mDesList[i + 1]+p, Math::Vector4::Black());
		}
	}
}

void TinyBot::DebugRender(Math::Vector2 p)
{
	//DrawScreenCircle({ mPosition + p,2.0f }, Math::Vector4::Magenta());

	char nn[6];
	_itoa_s(static_cast<int>(mDestination.x / 32), nn, 10);
	Graphics::DrawScreenText(nn, mPosition.x + p.x, mPosition.y + p.y, 14.0f, Math::Vector4::Magenta());
	_itoa_s(static_cast<int>(mDestination.y / 32), nn, 10);
	Graphics::DrawScreenText(nn, mPosition.x + 20.0f + p.x, mPosition.y + p.y, 14.0f, Math::Vector4::Magenta());
	
	_itoa_s(static_cast<int>(mStates), nn, 10);
	Graphics::DrawScreenText(nn, mPosition.x + 40.0f + p.x, mPosition.y + p.y, 14.0f, Math::Vector4::Black());

	/*_itoa_s(static_cast<int>(mIndex), nn, 10);
	DrawScreenText(nn, 240.0f, 60.0f, 24.0f, Math::Vector4::Magenta());*/
}

bool TinyBot::SearchEntity(ClassType type, BotStates state)
{
	auto cost = [](AI::Graph::Coord c1, AI::Graph::Coord c2)
	{
		float extraWeight = 0.0f;
		float diagonalExtra = sqrtf(2) - 1.0f;
		if (c1.x == c2.x || c1.y == c2.y)
		{
			diagonalExtra = 0.0f;
		}
		return 1.0f + extraWeight + diagonalExtra;
	};

	float tileSize = World().GetNavGraph().GetTileSize();
	AI::EntityList temp = World().GetEntities(static_cast<int>(type));
	if (!temp.empty())
	{
		AI::Graph::Coord aCoord = { static_cast<uint32_t>(Position().x / tileSize),static_cast<uint32_t>(Position().y / tileSize) };
		std::vector<AI::Graph::Coord> cTemp;
		for (auto& t : temp)
		{
			t->SetBlocked(false);
		}
		for (uint32_t i = 0; i < temp.size(); ++i)
		{
			cTemp.push_back({ static_cast<uint32_t>(temp[i]->Position().x / tileSize),static_cast<uint32_t>(temp[i]->Position().y / tileSize) });
		}

		if (World().GetNavGraph().RunDijkstra(aCoord, cTemp, cost))
		{
			SetDestinationList();
			mStates = state;
			mStateMachine->ChangeState(static_cast<int>(BotStates::Walk));
			for (auto& t : temp)
			{
				t->SetBlocked();
			}
			return true;
		}
	}
	return false;
}

void TinyBot::SetDestination(int x, int y)
{
	if ((x < 0 || x>40) || (y < 0 && y>23))
	{
		int temp = 0;
	}
	mDestination = { static_cast<float>(x) * mWorld.GetNavGraph().GetTileSize() + 0.5f*mWorld.GetNavGraph().GetTileSize(),
		static_cast<float>(y)* mWorld.GetNavGraph().GetTileSize() + 0.5f*mWorld.GetNavGraph().GetTileSize() };
}

void TinyBot::SetDestinationList()
{
	mDesList.clear();

	auto it = World().GetNavGraph().GetCloseList().back();
	AI::Graph::Node* temp = it.parent;
	float tileSize = World().GetNavGraph().GetTileSize();

	mDesList.emplace_back(Math::Vector2{ static_cast<float>(it.coord.x)*tileSize + tileSize * 0.5f,static_cast<float>(it.coord.y)*tileSize + tileSize * 0.5f });
	
	// If there is no Box
	if (temp != nullptr)
	{
		while (temp->parent != nullptr)
		{
			mDesList.emplace_back(Math::Vector2{ static_cast<float>(temp->coord.x)*tileSize + tileSize * 0.5f,static_cast<float>(temp->coord.y)*tileSize + tileSize * 0.5f });
			temp = temp->parent;
		};
	}
}

BotDirection TinyBot::GetDirection()
{
	if (mVelocity.y > 0)
	{
		return BotDirection::DOWN;
	}
	else if (mVelocity.y < 0)
	{
		return BotDirection::UP;
	}
	else
	{
		if (mVelocity.x > 0)
		{
			return BotDirection::RIGHT;
		}
		else
		{
			return BotDirection::LEFT;
		}
	}
	return BotDirection::DOWN;
}

// ====================== Class ======================
class IdleState :public AI::State<TinyBot>
{
public:
	float timer{ 1.0f };
	float tileSize{ 0.0f };
	void Enter(TinyBot& agent) override
	{
		tileSize = agent.World().GetNavGraph().GetTileSize();
		timer = 1.0f;
	}
	void Update(TinyBot& agent, float deltaTime) override
	{
		// Lambda Function
		auto cost = [](AI::Graph::Coord c1, AI::Graph::Coord c2)
		{
			float extraWeight = 0.0f;
			float diagonalExtra = sqrtf(2) - 1.0f;
			if (c1.x == c2.x || c1.y == c2.y)
			{
				diagonalExtra = 0.0f;
			}
			return 1.0f + extraWeight + diagonalExtra;
		};
		auto heuristic = [this, &agent](AI::Graph::Coord c)
		{
			uint32_t x = static_cast<uint32_t>(agent.Destination().x / tileSize) - c.x;
			uint32_t y = static_cast<uint32_t>(agent.Destination().y / tileSize) - c.y;
			return sqrtf(static_cast<float>(x * x + y * y));
		};

		// Random Walk
		{
			if (timer <= 0.0f)
			{
				int tempx = static_cast<int>(agent.Position().x / tileSize);
				int tempy = static_cast<int>(agent.Position().y / tileSize);

				agent.SetDestination(Math::Random(Math::Max(tempx - 2, 0), Math::Min(tempx + 2, static_cast<int>(agent.World().GetNavGraph().GetColumns()))),
					Math::Random(Math::Max(tempy - 2, 0), Math::Min(tempy + 2, static_cast<int>(agent.World().GetNavGraph().GetRows()))));
			}
			else
			{
				timer -= deltaTime;
			}
		}

		// RunAstar
		if (agent.Destination().x != agent.Position().x || agent.Destination().y != agent.Position().y)
		{
			if (agent.World().GetNavGraph().RunAStar({ static_cast<uint32_t>(agent.Position().x / tileSize),static_cast<uint32_t>(agent.Position().y / tileSize) },
				{ static_cast<uint32_t>(agent.Destination().x / tileSize),static_cast<uint32_t>(agent.Destination().y / tileSize) },
				cost, heuristic))
			{
				agent.SetDestinationList();
				agent.mStates = BotStates::Idle;
				agent.mStateMachine->ChangeState(static_cast<int>(BotStates::Walk));
			}
			else
			{
				agent.Destination() = agent.Position();
			}
		}
	}
	void Exit(TinyBot& agent) override {}
};

class WalkState :public AI::State<TinyBot>
{
public:
	int index{ 0 };
	int frame{ 1 };
	float timer{ ANIMATIONTIME };
	float checker{ DESTINATIONCHECK };
	void Enter(TinyBot& agent) override
	{
		agent.Destination() = agent.mDesList.back();
		agent.Velocity() = Math::Normalize(agent.Destination() - agent.Position())*agent.MaxSpeed();
		agent.mDesList.pop_back();

		index = 0;
		frame = 1;
		timer = ANIMATIONTIME;
		checker = DESTINATIONCHECK;

		agent.mDirection = agent.GetDirection();
		agent.mIndex = static_cast<int>(agent.mDirection)*agent.mTexCol + 1;
	}
	void Update(TinyBot& agent, float deltaTime) override
	{
		timer -= deltaTime;
		checker -= deltaTime;
		if (checker <= 0.0f)
		{
			agent.Velocity() = Math::Normalize(agent.Destination() - agent.Position())*agent.MaxSpeed();
		}
		// WARNNING: Can NOT change the Destination during the Moving
		if (Math::DistanceSqr(agent.Position(), agent.Destination()) > agent.MaxSpeed()*0.1f)
		{
			agent.Position() += agent.Velocity()*deltaTime;
		}
		else
		{
			agent.Position() = agent.Destination();
			if (!agent.mDesList.empty())
			{
				agent.Destination() = agent.mDesList.back();
				agent.Velocity() = Math::Normalize(agent.Destination() - agent.Position())*agent.MaxSpeed();
				agent.mDesList.pop_back();
			}
			checker = DESTINATIONCHECK;

			if (agent.mDirection != agent.GetDirection())
			{
				index = 0;
				frame = 1;
				timer = 0.25f;
				agent.mDirection = agent.GetDirection();
				agent.mIndex = static_cast<int>(agent.mDirection)*agent.mTexCol + 1;
			}
		}

		if (timer <= 0.0f)
		{
			timer = ANIMATIONTIME;
			++index;
			if (!static_cast<bool>(index % 2))
			{
				frame = -frame;
			}
			agent.mIndex += frame;
		}

		// Check Arrived and State
		if (agent.mDesList.empty() && Math::DistanceSqr(agent.Position(), agent.Destination()) <= agent.MaxSpeed()*0.1f)
		{
			agent.Position() = agent.Destination();
			switch (agent.mStates)
			{
			case BotStates::Idle:
				agent.mStateMachine->ChangeState(static_cast<int>(BotStates::Idle));
				break;
			case BotStates::Attack:
				agent.mStateMachine->ChangeState(static_cast<int>(BotStates::Attack));
				break;
			default:
				break;
			}
		}
	}
	void Exit(TinyBot& agent) override {}
};

class AttackState :public AI::State<TinyBot>
{
public:
	float attackAnimation{ 0.0f };
	float attackPeriod{ 0.0f };
	void Enter(TinyBot& agent) override
	{
		attackPeriod = agent.mAttackPeriod;
		attackAnimation = attackPeriod*agent.mAttackAnimationRate;
	}
	void Update(TinyBot& agent, float deltaTime) override
	{
		// TODO
		attackAnimation -= deltaTime;
		attackPeriod -= deltaTime;
		if (attackAnimation <= 0.0f)
		{

		}
		if (attackPeriod <= 0.0f)
		{
			agent.mStateMachine->ChangeState(static_cast<int>(BotStates::Idle));
		}
		Graphics::DrawScreenCircle({ agent.Position(),attackAnimation*4.0f }, Math::Vector4::Yellow());
		Graphics::DrawScreenCircle({ agent.Position(),attackPeriod*4.0f }, Math::Vector4::Magenta());
	}
	void Exit(TinyBot& agent) override {}
};
