#ifndef INCLUDED_AI_SEEKBEHAVIOR_H
#define INCLUDED_AI_SEEKBEHAVIOR_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

	class SeekBehavior : public SteeringBehavior
	{
	public:
		Math::Vector2 Calculate(Agent& agent) override;
		const char* GetName() const override { return "Seek"; }
	};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_SEEKBEHAVIOR_H