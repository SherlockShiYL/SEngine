#ifndef INCLUDED_AI_WALLAVOIDANCEBEHAVIOR_H
#define INCLUDED_AI_WALLAVOIDANCEBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

	class WallAvoidanceBehavior : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent) override;
		const char* GetName() const override { return "WallAvoidance"; }
		void SetElement(float d, float f) { maxVisionDistance = d, maxAvoidForce = f; }

		float maxVisionDistance;
		float maxAvoidForce;
	};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_WALLAVOIDANCEBEHAVIOR_H