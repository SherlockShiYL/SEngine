#ifndef INCLUDED_AI_AIWORLD_H
#define INCLUDED_AI_AIWORLD_H

#include "Agent.h"
#include "Graph.h"
#include "Entity.h"

namespace S {
namespace AI {

class AIWorld
{
public:
	typedef std::vector<Geometry::Circle> Obstacles;
	typedef std::vector<Geometry::LineSegment2D> Walls;

	AIWorld();
	~AIWorld();

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

	void RegisterEntity(Entity* entity);
	void UnregisterEntity(Entity* entity);

	AgentList GetNeighborhood(const Geometry::Circle& range);
	EntityList GetEntities(uint32_t type);

	void AddObstacles(const Geometry::Circle& obstacle);
	const Obstacles& GetObstacles() const { return mObstacles; }

	void AddWall(const Geometry::LineSegment2D& wall);
	const Walls& GetWalls() const { return mWalls; }

	Graph& GetNavGraph() { return mNavGraph; }
	const Graph& GetNavGraph() const { return mNavGraph; }

	bool HasLOS(const Geometry::LineSegment2D& line) const;

	/*void DebugRender()
	{
		for (auto& o : mObstacles)
		{
			DrawScreenCircle(o, Math::Vector4::Magenta());
		}
		for (auto& o : mWalls)
		{
			DrawScreenLine(o.from, o.to, Math::Vector4::Magenta());
		}
	}*/

private:
	AgentList mAgents;
	Obstacles mObstacles;
	Walls mWalls;
	Graph mNavGraph;
	bool mInitialized;
	EntityList mEntities;
};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_AIWORLD_H