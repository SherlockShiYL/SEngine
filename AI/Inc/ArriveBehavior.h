#ifndef INCLUDED_AI_ARRIVEBEHAVIOR_H
#define INCLUDED_AI_ARRIVEBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

class ArriveBehavior : public SteeringBehavior
{
public:
	Math::Vector2 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Arrive"; }
};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_ARRIVEBEHAVIOR_H