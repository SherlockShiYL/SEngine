#include "Precompiled.h"
#include "SteeringModule.h"

using namespace S;
using namespace AI;

SteeringModule::SteeringModule(Agent& agent)
	:mAgent(agent)
{}

SteeringModule::~SteeringModule()
{
	ASSERT(mBehaviors.empty(), "[SteeringModule] has no behaviors");
}

void SteeringModule::Purge()
{
	mBehaviors.clear();
}

S::Math::Vector2 SteeringModule::Calculate()
{
	S::Math::Vector2 total;
	for (auto& b : mBehaviors)
	{
		if (b->IsActive())
		{
			total += b->Calculate(mAgent);
		}
	}

	return total;
}