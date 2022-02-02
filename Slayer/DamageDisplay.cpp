#include "DamageDisplay.h"

using namespace S;

void DamageDisplay::Wake(int damage, Math::Vector2 position)
{
	mActive = true;
	_itoa_s(damage, mCharDamage, 10);
	mPosition = position;
	mTime = 1.0f;
}

void DamageDisplay::Update(float deltaTime)
{
	mTime -= deltaTime;
	if (mTime >= 0.0f)
	{
		mPosition += Math::Vector2{ 0.0f, -1.0f }*mSpeed*deltaTime;
	}
	else
	{
		mActive = false;
	}
}

void DamageDisplay::Render(Math::Vector2& p)
{
	Graphics::DrawScreenText(mCharDamage, mPosition + p, 24.0f, Math::Vector4::Magenta());
}
