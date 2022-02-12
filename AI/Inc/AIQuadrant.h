#ifndef INCLUDED_AI_QUADRANT_H
#define INCLUDED_AI_QUADRANT_H

#include "Agent.h"
#include "Entity.h"

namespace S {
namespace AI {

class AIWorld;

class AIQuadrant
{
public:
	AIQuadrant();

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

	void RegisterEntity(Entity* entity);
	void UnregisterEntity(Entity* entity);

	void Initialize(Math::Vector2 area, uint32_t rows, uint32_t columns);
	void Initialize(float width, float height, uint32_t columns, uint32_t rows);

	void Update();

	void DebugRender(Math::Vector2 p = { 0.0f,0.0f });

	uint32_t& GetRows()										{ return mRows; }
	const uint32_t& GetRows() const							{ return mRows; }

	uint32_t& GetColumns()									{ return mColumns; }
	const uint32_t& GetColumns() const						{ return mColumns; }

	float& GetWidth()										{ return mWidth; }
	const float& GetWidth() const							{ return mWidth; }

	float& GetHeight()										{ return mHeight; }
	const float& GetHeight() const							{ return mHeight; }

	float& GetRecipWidth()									{ return recipWidth; }
	const float& GetRecipWidth() const						{ return recipWidth; }

	float& GetRecipHeight()									{ return recipHeight; }
	const float& GetRecipHeight() const						{ return recipHeight; }

	std::vector<AgentList>& GetAgents()						{ return mAgentLists; }
	const std::vector<AgentList>& GetAgents() const			{ return mAgentLists; }

	std::vector<EntityList>& GetEntities()					{ return mEntityLists; }
	const std::vector<EntityList>& GetEntities() const		{ return mEntityLists; }

protected:
	float recipWidth; // Reciprocal of mWidth
	float recipHeight; // Reciprocal of mHeight
	float mWidth; // Width of each grid
	float mHeight; // Height of each grid
	uint32_t mRows;
	uint32_t mColumns;
	std::vector<AgentList> mAgentLists;
	std::vector<EntityList> mEntityLists;
};

} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_QUADRANT_H