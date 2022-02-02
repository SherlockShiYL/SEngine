#ifndef INCLUDED_SLAYER_BLACKHOLE_H
#define INCLUDED_SLAYER_BLACKHOLE_H

#include <Math/Inc/SMath.h>

class BlackHole
{
public:
	S::Math::Vector2 Attract(S::Math::Vector2 p);
	int Damage(int hp);


	const float GetGravityRadius() const			{ return mGravityRadius; }
	const S::Math::Vector2 GetPosition() const		{ return mPosition; }

private:
	bool mActive{ false };
	bool mMakeDamage{ false };
	int mDamage{ 20 };
	float mDamageCD{ 0.1f };
	float mGravityRadius{ 100.0f };
	float mDamageRadius{ 20.0f };
	float mMaxSpeed{ 20.0f };
	float mGravity{ 30.0f };
	S::Math::Vector2 mPosition{ 0.0f,0.0f };
};

#endif // !INCLUDED_SLAYER_BLACKHOLE_H