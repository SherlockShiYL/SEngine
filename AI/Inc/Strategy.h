#ifndef INCLUDED_AI_STRATEGY_H
#define INCLUDED_AI_STRATEGY_H

#include "Goal.h"

namespace S {
namespace AI {

	template <typename AgentType>
	class Strategy
	{
	public:
		Strategy(AgentType& agent) : mAgent(agent) {}
		virtual ~Strategy() {}

		virtual float CalculateDesirability() = 0;
		virtual Goal<AgentType>* CreateGoal() = 0;

	protected:
		AgentType& mAgent;
	};

} // namespace AI
} // namespace S

#endif // #ifndef INCLUDED_AI_STRATEGY_H