#include "Precompiled.h"
#include "SeekBehavior.h"
#include "Agent.h"

S::Math::Vector2 S::AI::SeekBehavior::Calculate(S::AI::Agent & agent)
{
	S::Math::Vector2 DesiredVelocity = S::Math::Normalize(agent.Destination() - agent.Position())*agent.MaxSpeed();
	return DesiredVelocity - agent.Velocity();
}