#include "Precompiled.h"
#include "CohesionBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

S::Math::Vector2 S::AI::CohesionBehavior::Calculate(S::AI::Agent & agent)
{
	AgentList agentList = agent.World().GetNeighborhood(Geometry::Circle{ agent.Position(), agent.MaxSpeed()*10.0f });
	Math::Vector2 groupPosition{ 0.0f,0.0f };
	for (auto& a : agentList)
	{
		if (a != &agent)
		{
			groupPosition += a->Position();
		}
	}
	if (agentList.size() != 1)
	{
		groupPosition /= static_cast<float>(agentList.size() - 1);
		Math::Vector2 DesiredVelocity = Math::Normalize(groupPosition - agent.Position())*agent.MaxSpeed();
		return DesiredVelocity - agent.Velocity();
	}
	return groupPosition;
}
