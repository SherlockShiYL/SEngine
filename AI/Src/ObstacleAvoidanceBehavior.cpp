#include "Precompiled.h"
#include "ObstacleAvoidanceBehavior.h"
#include "Agent.h"
#include "AIWorld.h"

S::Math::Vector2 S::AI::ObstacleAvoidanceBehavior::Calculate(S::AI::Agent & agent)
{
	S::Math::Vector2 norm = S::Math::Normalize(agent.Velocity());
	S::Math::Vector2 nnorm = S::Math::PerpendicularLH(norm);

	/*S::DrawScreenLine(agent.Position() - norm * 10.0f + nnorm * 10.0f, agent.Position() + norm * 100.0f + nnorm * 10.0f, S::Math::Vector4::LightBlue());
	S::DrawScreenLine(agent.Position() - norm * 10.0f + nnorm * 10.0f, agent.Position() - norm * 10.0f - nnorm * 10.0f, S::Math::Vector4::LightBlue());
	S::DrawScreenLine(agent.Position() + norm * 100.0f - nnorm * 10.0f, agent.Position() - norm * 10.0f - nnorm * 10.0f, S::Math::Vector4::LightBlue());
	S::DrawScreenLine(agent.Position() + norm * 100.0f - nnorm * 10.0f, agent.Position() + norm * 100.0f + nnorm * 10.0f, S::Math::Vector4::LightBlue());*/
	//S::DrawScreenCircle(S::Math::Circle(agent.Position(), agent.Radius()),S::Math::Vector4::LightGreen());

	AI::AIWorld::Obstacles obs = agent.World().GetObstacles();
	S::Math::Vector2 DesiredVelocity = { 0.0f,0.0f };
	for (auto& o : obs)
	{
		if (S::Math::Dot((o.center - agent.Position()), agent.Velocity()) > 0)
		{
			if (S::Math::DistanceSqr(o.center, agent.Position()) < (maxVisionDistance + o.radius)*(maxVisionDistance + o.radius))
			{
				if (S::Geometry::Intersect(S::Geometry::LineSegment2D(agent.Position(), agent.Position() + agent.Velocity()*agent.MaxSpeed()), S::Geometry::Circle(o.center, o.radius + agent.Radius())))
				{
					// There is problem, Agent will only avoid one obstacle
					DesiredVelocity = S::Math::Normalize(agent.Position() + agent.Velocity()*agent.MaxSpeed() - o.center)*maxAvoidForce;
				}
			}
		}
	}

	return DesiredVelocity;
}
