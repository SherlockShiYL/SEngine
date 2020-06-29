#ifndef INCLUDED_GAMEENGINE_GAMEOBJECTFACTORY_H
#define INCLUDED_GAMEENGINE_GAMEOBJECTFACTORY_H

#include "GameObject.h"

namespace S {

class GameObjectFactory
{
public:
	GameObjectFactory(GameObjectAllocator& allocator);

	GameObject* CreateTank();

	GameObject* Create(const char* templateFileName);
	void Destroy(GameObject* gameObject);

private:
	GameObjectAllocator& mGameObjectAllocator;
};

} // namespace S
#endif // !INCLUDED_GAMEENGINE_GAMEOBJECTFACTORY_H