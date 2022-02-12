#include "RedArrow.h"

using namespace S;

RedArrow::RedArrow(AI::AIWorld& myWorld)
	: Agent(myWorld)
{
	mSteeringModule = nullptr;
	mSeekBehavior = nullptr;
	mArriveBehavior = nullptr;
	mWanderBehavior = nullptr;
	mObsAvoBehavior = nullptr;
	mSeparationBehavior = nullptr;
	mCohesionBehavior = nullptr;
	mAlignmentBehavior = nullptr;
	mTextureId = 0;
}

void RedArrow::Load()
{
	mTextureId = Graphics::LoadTexture("AI_Generator_Images/RedArrow_S.png");
	mPosition = { Math::RandomFloat(100.0f,1200.0f), Math::RandomFloat(100.0f,620.0f) };
	mVelocity = { 0.1f,0.0f };
	mRadius = 20.0f;
	mMaxSpeed = 10.0f;

	mSteeringModule = new AI::SteeringModule(*this);
	// Default is false
	//mSteeringModule->AddBehavior<AI::SeekBehavior>()->SetActive(true);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();
	mObsAvoBehavior = mSteeringModule->AddBehavior<AI::ObstacleAvoidanceBehavior>();
	mAlignmentBehavior = mSteeringModule->AddBehavior<AI::AlignmentBehavior>();
	mCohesionBehavior = mSteeringModule->AddBehavior<AI::CohesionBehavior>();
	mSeparationBehavior = mSteeringModule->AddBehavior<AI::SeparationBehavior>();

	mWanderBehavior->SetElement(30.0f, 50.0f, 20.0f);
	mObsAvoBehavior->SetElement(mMaxSpeed, 10.0f);
	mSeekBehavior->SetActive(true);
	mObsAvoBehavior->SetActive(true);
}

float RedArrow::GetDirection()
{
	if (mVelocity.y >= 0.0f)
	{
		if (mVelocity.x > 0.0f)
		{
			return atan(mVelocity.y / mVelocity.x);
		}
		else if (mVelocity.x < 0.0f)
		{
			return Math::kPi + atan(mVelocity.y / mVelocity.x);
		}
		else
		{
			return Math::kPiByTwo;
		}
	}
	else
	{
		if (mVelocity.x < 0.0f)
		{
			return atan(mVelocity.y / mVelocity.x) + Math::kPi;
		}
		else if (mVelocity.x > 0.0f)
		{
			return Math::kPi + atan(mVelocity.y / mVelocity.x) + Math::kPi;
		}
		else
		{
			return Math::kPi + Math::kPiByTwo;
		}
	}
}

void RedArrow::ChangeBehavior(int index)
{
	switch (index)
	{
	case 1:
	{
		mSeekBehavior->SetActive(true);
		mArriveBehavior->SetActive(false);
		mWanderBehavior->SetActive(false);
		mAlignmentBehavior->SetActive(false);
		mCohesionBehavior->SetActive(false);
		mSeparationBehavior->SetActive(false);
		break;
	}
	case 2:
	{
		mSeekBehavior->SetActive(false);
		mArriveBehavior->SetActive(true);
		mWanderBehavior->SetActive(false);
		mAlignmentBehavior->SetActive(false);
		mCohesionBehavior->SetActive(false);
		mSeparationBehavior->SetActive(false);
		break;
	}
	case 3:
	{
		mSeekBehavior->SetActive(false);
		mArriveBehavior->SetActive(false);
		mWanderBehavior->SetActive(true);
		mAlignmentBehavior->SetActive(false);
		mCohesionBehavior->SetActive(false);
		mSeparationBehavior->SetActive(false);
		break;
	}
	case 4:
	{
		mObsAvoBehavior->SetActive(!mObsAvoBehavior->IsActive());
		break;
	}
	case 5:
	{
		mAlignmentBehavior->SetActive(!mAlignmentBehavior->IsActive());
		mCohesionBehavior->SetActive(!mCohesionBehavior->IsActive());
		mSeparationBehavior->SetActive(!mSeparationBehavior->IsActive());
		break;
	}
	default:
		break;
	}
	if (mAlignmentBehavior->IsActive() || mCohesionBehavior->IsActive() || mSeparationBehavior->IsActive())
	{
		mSeekBehavior->SetActive(false);
		mArriveBehavior->SetActive(false);
		mWanderBehavior->SetActive(true);
	}
}

void RedArrow::Update(float deltaTime, S::Math::Vector2 desition, bool b)
{
	mDestination = desition;
	mHeading = Math::Normalize(mVelocity);
	//DrawScreenLine(mPosition, mPosition + temp, Math::Vector4::Yellow());

	if (b)
		mVelocity += mSteeringModule->Calculate() * deltaTime; // Important
	mPosition += (mVelocity * mMaxSpeed * deltaTime);

	if (mPosition.x > static_cast<float>(Graphics::GetScreenWidth()) - 20.0f)
	{
		mPosition.x -= static_cast<float>(Graphics::GetScreenWidth()) - 40.0f;
		mDestination.x -= static_cast<float>(Graphics::GetScreenWidth()) - 40.0f;
	}
	else if (mPosition.x < 20.0f)
	{
		mPosition.x += static_cast<float>(Graphics::GetScreenWidth()) - 40.0f;
		mDestination.x += static_cast<float>(Graphics::GetScreenWidth()) - 40.0f;

	}
	if (mPosition.y > static_cast<float>(Graphics::GetScreenHeight()) - 20.0f)
	{
		mPosition.y -= (static_cast<float>(Graphics::GetScreenHeight()) - 40.0f);
		mDestination.y -= static_cast<float>(Graphics::GetScreenHeight()) - 40.0f;
	}
	else if (mPosition.y < 20.0f)
	{
		mPosition.y += static_cast<float>(Graphics::GetScreenHeight()) - 40.0f;
		mDestination.y += static_cast<float>(Graphics::GetScreenHeight()) - 40.0f;
	}
}

void RedArrow::Render(Math::Vector2 worldPosition)
{
	if (mObsAvoBehavior->IsActive())
	{
		mWorld.DebugRender(worldPosition);
	}

	Graphics::DrawSprite(mTextureId, mPosition + worldPosition, GetDirection() + Math::kPiByTwo);
	//X::DrawScreenLine(mPosition, mPosition + mVelocity*10.0f, X::Math::Vector4::Magenta());
}