#ifndef INCLUDED_AI_ALIGNMENTBEHAVIOR_H
#define INCLUDED_AI_ALIGNMENTBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

class AlignmentBehavior : public SteeringBehavior
{
public:
	S::Math::Vector2 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Alignment"; }
};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_ALIGNMENTBEHAVIOR_H