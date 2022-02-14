#ifndef INCLUDED_ENGINECODETEST_REDARROW_H
#define INCLUDED_ENGINECODETEST_REDARROW_H

#include <AI/Inc/AI.h>

class RedArrow final :public S::AI::Agent
{
public:
	RedArrow(S::AI::AIWorld& myWorld);
	~RedArrow()
	{
		//mSteeringModule->Purge();
		S::SafeDelete(mSteeringModule);
	}

	void Load();
	float GetDirection();
	void ChangeBehavior(int index);

	void Update(float deltaTime, S::Math::Vector2 desition, bool b);
	void Render(S::Math::Vector2 worldPosition = { 0.0f,0.0f });

private:
	S::AI::SteeringModule* mSteeringModule;
	S::AI::SeekBehavior* mSeekBehavior;
	S::AI::ArriveBehavior* mArriveBehavior;
	S::AI::WanderBehavior* mWanderBehavior;
	S::AI::ObstacleAvoidanceBehavior* mObsAvoBehavior;
	S::AI::SeparationBehavior* mSeparationBehavior;
	S::AI::CohesionBehavior* mCohesionBehavior;
	S::AI::AlignmentBehavior* mAlignmentBehavior;

	S::Graphics::TextureId mTextureId;
};

#endif // !INCLUDED_ENGINECODETEST_REDARROW_H