#include "Precompiled.h"
#include "ArriveBehavior.h"
#include "Agent.h"

S::Math::Vector2 S::AI::ArriveBehavior::Calculate(S::AI::Agent & agent)
{
	float slowingRadius = 150;
	//S::DrawScreenCircle(S::Math::Circle(agent.Position(), slowingRadius), S::Math::Vector4::Cyan());
	S::Math::Vector2 toTarget = agent.Destination() - agent.Position();
	float dist = S::Math::Distance(agent.Destination(), agent.Position());
	if (dist < slowingRadius)
	{
		toTarget = S::Math::Normalize(toTarget)*agent.MaxSpeed()*(dist / slowingRadius);
	}
	else
	{
		toTarget = S::Math::Normalize(toTarget)*agent.MaxSpeed();
	}
	return toTarget - agent.Velocity();
	/*if (dist > 0)
	{
		const double DecelTweaker = 0.3;
		double speed = dist / (static_cast<double>(-100.0f) * DecelTweaker);
		speed = S::Math::Min(speed, static_cast<double>(agent.MaxSpeed()));
		S::Math::Vector2 DesiredVelocity = toTarget * speed / dist;
		return (DesiredVelocity - agent.Velocity());
	}*/
}
