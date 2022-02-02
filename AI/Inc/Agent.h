#ifndef INCLUDED_AI_AGENT_H
#define INCLUDED_AI_AGENT_H

#include "Entity.h"

namespace S {
namespace AI {

class AIWorld;

class Agent : public Entity
{
public:
	Agent(AIWorld& world);
	virtual ~Agent();

	Math::Matrix3 LocalToWorld() const;

	Math::Vector2& Velocity()					{ return mVelocity; }
	const Math::Vector2& Velocity() const		{ return mVelocity; }

	Math::Vector2& Destination()				{ return mDestination; }
	const Math::Vector2& Destination() const	{ return mDestination; }

	Math::Vector2& Heading()					{ return mHeading; }
	const Math::Vector2& Heading() const		{ return mHeading; }

	float& MaxSpeed()							{ return mMaxSpeed; }
	float MaxSpeed() const						{ return mMaxSpeed; }

	float& Radius()								{ return mRadius; }
	float Radius() const						{ return mRadius; }

protected:
	Math::Vector2 mVelocity;
	Math::Vector2 mDestination;
	Math::Vector2 mHeading;

	float mRadius;
	float mMaxSpeed;
};

using AgentList = std::vector<Agent*>;

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_AGENT_H