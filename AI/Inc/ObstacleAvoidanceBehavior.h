#ifndef INCLUDED_AI_OBSAVOIDANCEBEHAVIOR_H
#define INCLUDED_AI_OBSAVOIDANCEBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

	class ObstacleAvoidanceBehavior : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent) override;
		const char* GetName() const override { return "ObsAvoidance"; }
		void SetElement(float d, float f) { maxVisionDistance = d * 10.0f, maxAvoidForce = f; }

		float maxVisionDistance;
		float maxAvoidForce;
	};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_OBSAVOIDANCEBEHAVIOR_H