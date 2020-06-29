#include "Precompiled.h"
#include "SeparationBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

S::Math::Vector2 S::AI::SeparationBehavior::Calculate(S::AI::Agent & agent)
{
	AgentList agentList = agent.World().GetNeighborhood(S::Geometry::Circle{ agent.Position(), agent.MaxSpeed()*10.0f });
	S::Math::Vector2 DesiredVelocity{ 0.0f,0.0f };
	for (auto& a : agentList)
	{
		if (a != &agent)
		{
			if (a->Position().x != agent.Position().x && a->Position().y != agent.Position().y)
			{
				DesiredVelocity += S::Math::Normalize(agent.Position() - a->Position())*agent.MaxSpeed() / S::Math::Distance(agent.Position(), a->Position());
			}
		}
	}

	return DesiredVelocity * 5.0f;
}
