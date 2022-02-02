#include "Precompiled.h"
#include "AIWorld.h"

using namespace S;
using namespace S::AI;

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

	if (range.center.x >= 0.0f && range.center.x <= static_cast<float>(mAIQuadrant.GetColumns())*mAIQuadrant.GetWidth()
		&& range.center.y >= 0.0f && range.center.y <= static_cast<float>(mAIQuadrant.GetRows())*mAIQuadrant.GetHeight())
	{
		uint32_t columnIndex = static_cast<int>(range.center.x*mAIQuadrant.GetRecipWidth());
		uint32_t rowIndex = static_cast<int>(range.center.y*mAIQuadrant.GetRecipHeight());

		uint32_t index = columnIndex + mAIQuadrant.GetColumns() * rowIndex;

		int widthRange = static_cast<int>(range.radius / mAIQuadrant.GetWidth()) + 1;
		int heightRange = static_cast<int>(range.radius / mAIQuadrant.GetHeight()) + 1;

		for (int i = -widthRange; i <= widthRange; ++i)
		{
			for (int j = -heightRange; j <= heightRange; ++j)
			{
				if (columnIndex + i < 0 || columnIndex + i > mAIQuadrant.GetColumns() - 1
					|| rowIndex + j < 0 || rowIndex + j > mAIQuadrant.GetRows() - 1)
				{
					for (auto& agent : mAIQuadrant.GetAgents()[mAIQuadrant.GetAgents().size() - 1])
					{
						if (Math::DistanceSqr(range.center, agent->Position()) < range.radius*range.radius)
						{
							temp.push_back(agent);
						}
					}
				}
				else
				{
					for (auto& agent : mAIQuadrant.GetAgents()[index + i + mAIQuadrant.GetColumns()*j])
					{
						if (Math::DistanceSqr(range.center, agent->Position()) < range.radius*range.radius)
						{
							temp.push_back(agent);
						}
					}
				}
			}
		}
	}
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

Agent* AI::AIWorld::GetOneAgent(uint32_t type)
{
	for (size_t i = 0; i < mAgents.size(); ++i)
	{
		if (mAgents[i]->Type() == type)
		{
			return mAgents[i];
		}
	}
	return nullptr;
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
