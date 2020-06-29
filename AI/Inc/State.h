#ifndef INCLUDED_AI_STATE_H
#define INCLUDED_AI_STATE_H

namespace S {
namespace AI {

	template <class AgentType>
	class State
	{
	public:
		virtual ~State() {}
		virtual void Enter(AgentType& agent) = 0;
		virtual void Update(AgentType& agent, float deltaTime) = 0;
		virtual void Exit(AgentType& agent) = 0;
	};

} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_STATE_H
