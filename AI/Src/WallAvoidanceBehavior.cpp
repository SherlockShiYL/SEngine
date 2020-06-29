#include "Precompiled.h"
#include "WallAvoidanceBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

S::Math::Vector2 S::AI::WallAvoidanceBehavior::Calculate(S::AI::Agent & agent)
{
	//S::DrawScreenLine(agent.Position(), { agent.Position().x + agent.Velocity().x*maxVisionDistance,agent.Position().y + agent.Velocity().y*maxVisionDistance }, S::Math::Vector4::Magenta());
	AI::AIWorld::Walls walls = agent.World().GetWalls();
	S::Math::Vector2 DesiredVelocity = { 0.0f,0.0f };
	for (auto& o : walls)
	{
		if (S::Geometry::Intersect(o, S::Geometry::LineSegment2D(agent.Position(), { agent.Position().x + agent.Velocity().x * maxVisionDistance,agent.Position().y + agent.Velocity().y*maxVisionDistance })))
		{
			if (o.from.y - o.to.y == 0.0f)
			{
				DesiredVelocity = { 0.0f,o.from.y - (agent.Position().y + agent.Velocity().y* maxVisionDistance) };
			}
			else
			{
				DesiredVelocity = { o.from.x - (agent.Position().x + agent.Velocity().x* maxVisionDistance),0.0f };
			}
		}
	}
	return DesiredVelocity;
}
