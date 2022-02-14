#ifndef INCLUDED_AI_STEERINGBEHAVIOR_H
#define INCLUDED_AI_STEERINGBEHAVIOR_H

namespace S {
namespace AI {

class Agent;

class SteeringBehavior
{
public:
	SteeringBehavior() : mActive(false) {}
	virtual ~SteeringBehavior() {}

	virtual S::Math::Vector2 Calculate(Agent& agent) = 0;
	virtual const char* GetName() const = 0;

	void SetActive(bool active) { mActive = active; }
	bool IsActive() const { return mActive; }

private:
	bool mActive;
};

} // namespace AI
} // namespace S

#endif // !INCLUDED_AI_STEERINGBEHAVIOR_H
