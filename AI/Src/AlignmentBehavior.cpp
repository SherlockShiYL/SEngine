#include "Precompiled.h"
#include "AlignmentBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

S::Math::Vector2 S::AI::AlignmentBehavior::Calculate(S::AI::Agent & agent)
{
	AgentList agentList = agent.World().GetNeighborhoodQuadrant(Geometry::Circle{ agent.Position(), agent.MaxSpeed()*10.0f });
	Math::Vector2 DesiredVelocity{ 0.0f,0.0f };
	for (auto& a : agentList)
	{
		if (a != &agent)
		{
			DesiredVelocity += a->Velocity();
		}
	}
	if (agentList.size() != 1)
	{
		DesiredVelocity = Math::Normalize(DesiredVelocity / static_cast<float>((agentList.size() - 1)));
		return (DesiredVelocity - agent.Heading())*10.0f;
	}
	return DesiredVelocity;
}