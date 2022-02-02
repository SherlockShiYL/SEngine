#ifndef INCLUDED_SLAYER_SOURCE_H
#define INCLUDED_SLAYER_SOURCE_H

#include <Math/Inc/SMath.h>

class Source
{
public:
	void Load(int wood,int gold, int rock, int iron);
	void Update(float deltaTime);
	void Render();

	const int GetWood() const { return mWood; }
	const int GetGold() const { return mGold; }
	const int GetRock() const { return mRock; }
	const int GetIron() const { return mIron; }

private:
	int mWood{ -1 };
	int mGold{ -1 };
	int mRock{ -1 };
	int mIron{ -1 };
};


#endif // !INCLUDED_SLAYER_SOURCE_H