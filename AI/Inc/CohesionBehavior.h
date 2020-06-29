#ifndef INCLUDED_AI_COHESIONBEHAVIOR_H
#define INCLUDED_AI_COHESIONBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

	class CohesionBehavior : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent) override;
		const char* GetName() const override { return "Cohesion"; }
	};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_COHESIONBEHAVIOR_H