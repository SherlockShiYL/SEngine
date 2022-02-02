#include "ShivLighting.h"

using namespace S;

void ShivLighting::Wake(int d, uint32_t type, uint32_t id, Math::Vector2 p)
{
	mActive = true;
	mDamage = d;
	mType = type;
	mId = id;
	mPosition = p;
}
