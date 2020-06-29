#ifndef INCLUDED_AI_STEERINGMODULE_H
#define INCLUDED_AI_STEERINGMODULE_H

#include "SteeringBehavior.h"

namespace S {
namespace AI {

	class SteeringModule
	{
	public:
		SteeringModule(Agent& agent);
		~SteeringModule();

		template <class BehaviorType>
		BehaviorType* AddBehavior();
		template <class BehaviorType>
		BehaviorType* GetBehavior(const char* name);
		void Purge();

		Math::Vector2 Calculate();

	private:
		Agent & mAgent;
		std::vector<std::unique_ptr<SteeringBehavior>> mBehaviors;
	};

	template <class BehaviorType>
	BehaviorType* SteeringModule::AddBehavior()
	{
		mBehaviors.push_back(std::make_unique<BehaviorType>());
		return static_cast<BehaviorType*>(mBehaviors.back().get());
	}

	template <class BehaviorType>
	BehaviorType* SteeringModule::GetBehavior(const char* name)
	{
		for (auto& b : mBehaviors)
		{
			if (strcmp(b->GetName(), name) == 0)
			{
				return static_cast<BehaviorType*>(b.get());
			}
		}

		return nullptr;
	}

} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_STEERINGMODULE_H
