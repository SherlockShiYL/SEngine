#include "Precompiled.h"
#include "AIWorld.h"

using namespace S;
using namespace S::AI;

AIWorld::AIWorld()
{
}

AIWorld::~AIWorld()
{
}

void AIWorld::RegisterAgent(Agent* agent)
{
	mAgents.push_back(agent);
}

void AIWorld::UnregisterAgent(Agent* agent)
{
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end())
	{
		mAgents.erase(iter);
	}
}

void AIWorld::RegisterEntity(Entity * entity)
{
	mEntities.push_back(entity);
}

void AIWorld::UnregisterEntity(Entity * entity)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end())
	{
		mEntities.erase(iter);
	}
}

AgentList AIWorld::GetNeighborhood(const Geometry::Circle & range)
{
	AgentList temp;
	for (auto& a : mAgents)
	{
		if (Math::DistanceSqr(range.center, a->Position()) < range.radius*range.radius)
		{
			temp.push_back(a);
		}
	}
	//S::DrawScreenCircle(S::Math::Circle({ range.center,range.radius }), S::Math::Vector4::Cyan());
	return temp;
} // Copy Elision

EntityList AIWorld::GetEntities(uint32_t type)
{
	EntityList temp;
	for (size_t i = 0; i < mEntities.size(); ++i)
	{
		if (mEntities[i]->Type() == type)
		{
			temp.push_back(mEntities[i]);
		}
	}
	return temp;
}

void AIWorld::AddObstacles(const Geometry::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld::AddWall(const Geometry::LineSegment2D& wall)
{
	mWalls.push_back(wall);
}

bool AIWorld::HasLOS(const Geometry::LineSegment2D& line) const
{
	for (size_t i = 0; i < AI::AIWorld::GetWalls().size(); ++i)
	{
		if (Geometry::Intersect(line, AI::AIWorld::GetWalls()[i]))
		{
			return false;
		}
	}
	return true;
}
