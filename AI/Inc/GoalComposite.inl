using namespace S;
using namespace S::AI;

template <typename AgentType>
GoalComposite<AgentType>::GoalComposite(AgentType& agent)
	: Goal<AgentType>(agent)
{
}

template <typename AgentType>
GoalComposite<AgentType>::~GoalComposite()
{
	RemoveAllSubgoals();
}

template <typename AgentType>
void GoalComposite<AgentType>::AddSubgoal(Goal<AgentType>* goal)
{
	mSubgoals.push_front(goal);
}

template <typename AgentType>
void GoalComposite<AgentType>::RemoveAllSubgoals()
{
	for (auto goal : mSubgoals)
	{
		if (goal != nullptr)
		{
			goal->Terminate();
			SafeDelete(goal);
		}
	}
	mSubgoals.clear();
}

template <typename AgentType>
typename Goal<AgentType>::Status GoalComposite<AgentType>::ProcessSubgoals()
{
	// Remove all completed goals
	while (!mSubgoals.empty())
	{
		Goal<AgentType>* goal = mSubgoals.front();
		if (goal->mStatus != Goal<AgentType>::Status::Completed &&
			goal->mStatus != Goal<AgentType>::Status::Failed)
		{
			break;
		}

		goal->Terminate();
		SafeDelete(goal);
		mSubgoals.pop_front();
	}

	// If any Goals remain, continue to process
	if (!mSubgoals.empty())
	{
		typename Goal<AgentType>::Status status = mSubgoals.front()->Process();
		if (status == Goal<AgentType>::Status::Completed && mSubgoals.size() > 1)
		{
			return Goal<AgentType>::Status::Active;
		}
		else
		{
			return status;
		}
	}
	return Goal<AgentType>::Status::Completed;
}