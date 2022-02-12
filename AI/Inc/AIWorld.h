#ifndef INCLUDED_AI_AIWORLD_H
#define INCLUDED_AI_AIWORLD_H

#include "Agent.h"
#include "AIQuadrant.h"
#include "AIQuadtree.h"
#include "Graph.h"
#include "Entity.h"

namespace S {
namespace AI {

class AIWorld
{
public:
	typedef std::vector<Geometry::Circle> Obstacles;
	typedef std::vector<Geometry::LineSegment2D> Walls;

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

	void RegisterEntity(Entity* entity);
	void UnregisterEntity(Entity* entity);

	AgentList GetNeighborhoodQuadrant(const Geometry::Circle& range);
	//AgentList GetNeighborhoodQuadtree(const Geometry::Circle& range);

	const AgentList& GetAgents() const			{ return mAgents; }
	Agent* GetOneAgent(uint32_t type);

	const EntityList& GetEntities() const		{ return mEntities; }
	EntityList GetEntities(uint32_t type);

	void AddObstacles(const Geometry::Circle& obstacle);
	const Obstacles& GetObstacles() const		{ return mObstacles; }

	void AddWall(const Geometry::LineSegment2D& wall);
	const Walls& GetWalls() const				{ return mWalls; }

	Graph& GetNavGraph()						{ return mNavGraph; }
	const Graph& GetNavGraph() const			{ return mNavGraph; }

	AIQuadrant& GetQuadrant()					{ return mAIQuadrant; }
	const AIQuadrant& GetQuadrant() const		{ return mAIQuadrant; }

	/*AIQuadtree& GetQuadtree()					{ return mAIQuadtree; }
	const AIQuadtree& GetQuadtree() const		{ return mAIQuadtree; }*/

	void Update();

	bool HasLOS(const Geometry::LineSegment2D& line) const;

	void DebugRender(Math::Vector2 worldPosition = { 0.0f,0.0f })
	{
		for (auto& o : mObstacles)
		{
			Graphics::DrawScreenCircle(o + worldPosition, Math::Vector4::Magenta());
		}
		for (auto& o : mWalls)
		{
			Graphics::DrawScreenLine(o.from + worldPosition, o.to + worldPosition, Math::Vector4::Magenta());
		}
	}

private:
	//bool mInitialized{ false };
	Obstacles mObstacles;
	Walls mWalls;
	Graph mNavGraph;
	AIQuadrant mAIQuadrant;
	//AIQuadtree mAIQuadtree;
	AgentList mAgents;
	EntityList mEntities;
};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_AIWORLD_H