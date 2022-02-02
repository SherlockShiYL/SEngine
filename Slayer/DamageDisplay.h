#ifndef INCLUDED_SLAYER_DAMAGEDISPLAY_H
#define INCLUDED_SLAYER_DAMAGEDISPLAY_H

#include <GameEngine/Inc/GameEngine.h>

struct ServerDamageDisplay
{
	int mDamage{ 0 };
	S::Math::Vector2 mPosition;
};

class DamageDisplay
{
public:
	void Wake(int damage, S::Math::Vector2 position);
	void Update(float deltaTime);
	void Render(S::Math::Vector2& p);
	bool IsActive()				{ return mActive; }

private:
	bool mActive{ false };
	float mTime{ 1.0f };
	float mSpeed{ 20.0f };
	S::Math::Vector2 mPosition;
	char mCharDamage[16];
};

using ServerDamageDisplayList = std::vector<ServerDamageDisplay>;

#endif // !INCLUDED_SLAYER_DAMAGEDISPLAY_H