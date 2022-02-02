#ifndef INCLUDED_PHYSICS_COLLIDERMANAGER_H
#define INCLUDED_PHYSICS_COLLIDERMANAGER_H

#include "CircleCollider.h"

namespace S {
namespace Physics {

class ColliderManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ColliderManager* Get();

	//ColliderManager(){}

	void AddCollider(CircleCollider cCollider, uint32_t type);
	void Update();
	void CheckCollision();
	void Render(Math::Vector2 p, float scale);

protected:
	std::vector<std::pair<uint32_t, std::vector<CircleCollider>>> mCircleBounds;

};

} // namespace Physics
} // namespace S

#endif // !INCLUDED_PHYSICS_COLLIDERMANAGER_H