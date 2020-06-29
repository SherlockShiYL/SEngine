#ifndef INCLUDED_PATHPLANNER_H
#define INCLUDED_PATHPLANNER_H

#include "Graph.h"

namespace S {
namespace AI {

class Agent;

class PathPlanner
{
public:
	PathPlanner(Agent& agent);
	~PathPlanner();
	using Path = std::vector<Math::Vector2>;
	using CostFunc = Graph::CostFunc;
	using HeuristicFunc = Graph::HeuristicFunc;

	void RequestPath(CostFunc g, HeuristicFunc h);
	void RequestPath(uint32_t type, CostFunc g);
	const Path& GetPath() const { return mPath; }

private:
	Agent & mAgent;
	Path mPath;
};

} // namespace AI
} // namespace S

#endif // !INCLUDED_PATHPLANNER_H