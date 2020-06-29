#include "Precompiled.h"
#include "WanderBehavior.h"
#include "Agent.h"

S::Math::Vector2 S::AI::WanderBehavior::Calculate(S::AI::Agent & agent)
{
	S::Math::Vector2 circleCenter = agent.Position() + S::Math::Normalize(agent.Velocity()) * WanderDistance;
	int randNum = rand() % 360;
	float mx = cos(randNum * S::Math::kDegToRad)*maxChangeRadius;
	float my = sin(randNum * S::Math::kDegToRad)*maxChangeRadius;
	target = circleCenter + S::Math::Normalize({ target.x + mx - circleCenter.x ,target.y + my - circleCenter.y })*WanderRadius;
	S::Math::Vector2 DesiredVelocity = S::Math::Normalize(target - agent.Position())*agent.MaxSpeed();

	/*S::DrawScreenCircle(S::Math::Circle(circleCenter, WanderRadius), S::Math::Vector4::Yellow());
	S::DrawScreenLine(circleCenter, target, S::Math::Vector4::Yellow());
	S::DrawScreenLine(target, agent.Position(), S::Math::Vector4::Yellow());*/

	return DesiredVelocity - agent.Velocity();
}