#include "Precompiled.h"
#include "Agent.h"
#include "PathPlanner.h"
#include "AIWorld.h"

using namespace S;
using namespace AI;

Agent::Agent(AIWorld& world)
	: Entity{ world }
	, mVelocity{ 0.0f, 0.0f }
	, mDestination{ 0.0f, 0.0f }
	, mHeading{ 0.0f, 1.0f }
	, mMaxSpeed{ 0.0f }
{
	mWorld.RegisterAgent(this);
}

Agent::~Agent()
{
	mWorld.UnregisterAgent(this);
}

Math::Matrix3 Agent::LocalToWorld() const
{
	return Math::Matrix3(
		-mHeading.y, mHeading.x, 0.0f,
		mHeading.x, mHeading.y, 0.0f,
		mPosition.x, mPosition.y, 1.0f
	);
}