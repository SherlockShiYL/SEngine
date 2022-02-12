#ifndef INCLUDED_AI_SEPARATIONBEHAVIOR_H
#define INCLUDED_AI_SEPARATIONBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

class SeparationBehavior : public SteeringBehavior
{
public:
	Math::Vector2 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Separation"; }
};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_SEPARATIONBEHAVIOR_H