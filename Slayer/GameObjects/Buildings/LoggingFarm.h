#ifndef INCLUDED_SLAYER_LOGGINGFARM_H
#define INCLUDED_SLAYER_LOGGINGFARM_H

#include "Building.h"

class LoggingFarm :public Building
{
public:
	LoggingFarm(int hp, S::Math::Vector2 position, uint32_t level, int armor
		, float countdown, float incomeamount, S::Geometry::Rect rect)
		: Building(hp, position, level, armor, countdown, incomeamount, rect)
	{}

	void Update(float deltaTime);
	void Render(S::Math::Vector2 p, float scale);

	void LoadTexture() override;
	void Load(S::Math::Vector2 position) override;

private:

};


#endif // !INCLUDED_SLAYER_LOGGINGFARM_H
