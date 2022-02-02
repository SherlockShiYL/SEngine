#include "Marble.h"

using namespace S;
using namespace S::Graphics;

void Marble::Load()
{
	mPosition = { 300.0f,300.0f };
	mId = LoadTexture("bullet2.png");
}

void Marble::InitializeVelocity()
{
	if (Math::Random(0, 1))
		mVelocity.y = 1.0f;
	else
		mVelocity.y = -1.0f;
	mVelocity.x = Math::RandomFloat(-1.0f, 1.0f);
	mVelocity = Math::Normalize(mVelocity);
}

void Marble::Update(float deltaTime)
{
	mPosition += mVelocity * deltaTime * mSpeed;
	if (mPosition.x - mRadius <= 0.0f)
	{
		mPosition.x = Math::Abs(mPosition.x - mRadius) + mRadius;
		mVelocity.x = -mVelocity.x;
	}
	else if (mPosition.x + mRadius > static_cast<float>(GraphicsSystem::Get()->GetWidth()))
	{
		mPosition.x = static_cast<float>(GraphicsSystem::Get()->GetWidth()) -
			(mPosition.x + mRadius - static_cast<float>(GraphicsSystem::Get()->GetWidth())) - mRadius;
		mVelocity.x = -mVelocity.x;
	}

	if (mPosition.y <= 0.0f || mPosition.y >= static_cast<float>(GraphicsSystem::Get()->GetHeight()))
		mVelocity = Math::Vector2::Zero();

}

void Marble::Render()
{
	DrawSprite(mId, mPosition);
	SimpleDraw::AddScreenCircle({ mPosition,mRadius }, Math::Vector4::Cyan());
}
