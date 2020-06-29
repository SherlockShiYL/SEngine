#ifndef INCLUDED_AI_WANDERBEHAVIOR_H
#define INCLUDED_AI_WANDERBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

	class WanderBehavior : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent) override;
		const char* GetName() const override { return "Wander"; }
		void SetElement(float r, float d, float max) { WanderRadius = r, WanderDistance = d, target = { 0.0f,0.0f }, maxChangeRadius = max; }

		Math::Vector2 target;
		float WanderRadius;
		float WanderDistance;
		float maxChangeRadius;
	};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_WANDERBEHAVIOR_H