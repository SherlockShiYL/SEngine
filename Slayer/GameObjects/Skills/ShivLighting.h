#ifndef INCLUDED_SLAYER_SHIVLIGHTING_H
#define INCLUDED_SLAYER_SHIVLIGHTING_H

#include <Math/Inc/SMath.h>

class ShivLighting
{
public:
	const bool IsAlive() const				{ return mActive; }
	void Wake(int d, uint32_t type, uint32_t id, S::Math::Vector2 p);

	int GetDamage()							{ return mDamage; }

	// Ask Enemy or Player give the next target position;
	void GetPositionList(S::Math::Vector2 pl);

	void Render(S::Math::Vector2 p);

private:
	bool mActive{ false };
	int mDamage{ 0 };
	uint32_t mType{ 0 }; // Enemy's 1 or Player's 0
	uint32_t mId{ 0 };
	float mRadius{ 100.0f }; // Might need a construct to set default value
	S::Math::Vector2 mPosition{ 0.0f,0.0f };
};

#endif // !INCLUDED_SLAYER_SHIVLIGHTING_H