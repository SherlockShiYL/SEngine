#ifndef INCLUDED_AI_QUADTREE_H
#define INCLUDED_AI_QUADTREE_H

#include "Agent.h"
#include "Entity.h"

namespace S {
namespace AI {

class AIWorld;

class AIQuadtree
{
public:
	void Initialize(Geometry::Rect rect, uint32_t capacity);
	void Terminate();

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

	void RegisterEntity(Entity* entity);
	void UnregisterEntity(Entity* entity);

	void Subdivide();
	bool Insert(Agent* agent);
	bool Insert(Entity* entity);
	bool Erase(Agent* agent);
	bool Erase(Entity* entity);

	void QueryRange(AgentList& result, S::Geometry::Rect rect);
	void QueryRange(AgentList& result, S::Geometry::Circle circle);
	void QueryRange(EntityList& result, S::Geometry::Rect rect);
	void QueryRange(EntityList& result, S::Geometry::Circle circle);

	bool Empty();
	void CheckValid(AgentList& result);
	void CheckValid(EntityList& result);
	void Update();
	void DeleteEmptyQuadtrees();

	void DebugRender(S::Math::Vector2 p = { 0.0f,0.0f });


	/*std::vector<AgentList>& GetAgents()						{ return mAgents; }
	const std::vector<AgentList>& GetAgents() const			{ return mAgents; }

	std::vector<EntityList>& GetEntities()					{ return mEntities; }
	const std::vector<EntityList>& GetEntities() const		{ return mEntities; }*/

protected:
	AIQuadtree* mTL{ nullptr };
	AIQuadtree* mTR{ nullptr };
	AIQuadtree* mBL{ nullptr };
	AIQuadtree* mBR{ nullptr };

	uint32_t mCapacity; // Max amount in this area
	Geometry::Rect mRect; // Quadtree range

	AgentList mAgents;
	EntityList mEntities;
};

} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_QUADTREE_H