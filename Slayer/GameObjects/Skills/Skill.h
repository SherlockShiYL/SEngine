#ifndef INCLUDED_SLAYER_SKILL_H
#define INCLUDED_SLAYER_SKILL_H

#include <GameEngine/Inc/GameEngine.h>
#include "../Player.h"

class Skill
{
public:
	void Initialize();

	void SetDamage(uint32_t lvl); // Set damage depends on the lvl

	/*void Heal(Player* player) 
	{
		
	}*/

private:
	bool mActive{ false };
	bool mBuff;
	bool mDeBuff;
	int mDamage{ 0 };
	uint32_t mLevel{ 0 };
	uint32_t mId{ 0u };
	uint32_t mType{ 0u }; // What kind of skill is this
	float mTime{ 1.0f };
	float mActiveTime;
	float mCD;
	float mDamageTime;
	float mRadius{ 10.0f };
	float mSpeed{ 0.0f };
	S::Math::Vector2 mHeading{ 1.0f,0.0f };
	S::Math::Vector2 mPosition{ 0.0f,0.0f };

};

#endif // !INCLUDED_SLAYER_SKILL_H