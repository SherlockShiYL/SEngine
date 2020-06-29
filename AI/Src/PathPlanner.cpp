#include "Precompiled.h"
#include "PathPlanner.h"
#include "AIWorld.h"
#include "Agent.h"

using namespace S::AI;

PathPlanner::PathPlanner(Agent& agent)
	:mAgent(agent)
{
}

PathPlanner::~PathPlanner()
{
}

void PathPlanner::RequestPath(CostFunc g, HeuristicFunc h)
{
	mPath.clear();
	if (mAgent.World().HasLOS(S::Geometry::LineSegment2D{ mAgent.Position(), mAgent.Destination() }))
	{
		mPath = { mAgent.Position(),mAgent.Destination() };
		return;
	}
	float tSize = mAgent.World().GetNavGraph().GetTileSize();
	AI::Graph::Coord aCoord = { static_cast<uint32_t>(mAgent.Position().x / tSize),static_cast<uint32_t>(mAgent.Position().y / tSize) };
	AI::Graph::Coord dCoord = { static_cast<uint32_t>(mAgent.Destination().x / tSize),static_cast<uint32_t>(mAgent.Destination().y / tSize) };
	if (!mAgent.World().GetNavGraph().RunAStar(aCoord, dCoord, g, h))
	{
		return;
	}
	mPath = mAgent.World().GetNavGraph().GetPosPath();
	mPath.insert(mPath.begin(), mAgent.Position());
	mPath.push_back(mAgent.Destination());

	size_t first = 0;
	size_t check = 2;
	while (check < mPath.size())
	{
		if (mAgent.World().HasLOS(S::Geometry::LineSegment2D(mPath[first], mPath[check])))
		{
			mPath.erase(mPath.begin() + first + 1);
		}
		else
		{
			++first;
			++check;
		}
	}
}

void PathPlanner::RequestPath(uint32_t type, CostFunc g)
{
	AI::EntityList temp = mAgent.World().GetEntities(type);
	if (temp.empty())
	{
		return;
	}
	float tSize = mAgent.World().GetNavGraph().GetTileSize();
	AI::Graph::Coord aCoord = { static_cast<uint32_t>(mAgent.Position().x / tSize),static_cast<uint32_t>(mAgent.Position().y / tSize) };
	std::vector<Graph::Coord> cTemp;
	for (size_t i = 0; i < temp.size(); ++i)
	{
		cTemp.push_back({ static_cast<uint32_t>(temp[i]->Position().x / tSize),static_cast<uint32_t>(temp[i]->Position().y / tSize) });
	}
	if (!mAgent.World().GetNavGraph().RunDijkstra(aCoord, cTemp, g))
	{
		return;
	}

	mPath = mAgent.World().GetNavGraph().GetPosPath();
	mPath.insert(mPath.begin(), mAgent.Position());

	size_t first = 0;
	size_t check = 2;
	while (check < mPath.size())
	{
		if (mAgent.World().HasLOS(S::Geometry::LineSegment2D(mPath[first], mPath[check])))
		{
			mPath.erase(mPath.begin() + first + 1);
		}
		else
		{
			++first;
			++check;
		}
	}
}