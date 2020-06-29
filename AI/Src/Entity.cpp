#include "Precompiled.h"
#include "Entity.h"
#include "AIWorld.h"

using namespace S::AI;

Entity::Entity(AIWorld & world)
	:mWorld(world)
{
	mWorld.RegisterEntity(this);
}

Entity::~Entity()
{
	mWorld.UnregisterEntity(this);
}

void Entity::Load(S::Math::Vector2 p, uint32_t type, uint32_t id)
{
	mPosition = p;
	mType = type;
	mId = id;
}

void Entity::Load(float tileSize, int columns, int rows, uint32_t type, uint32_t id)
{
	mPosition.x = tileSize * static_cast<float>(columns) + 0.5f*static_cast<float>(tileSize);
	mPosition.y = tileSize * static_cast<float>(rows) + 0.5f*static_cast<float>(tileSize);
	mType = type;
	mId = id;
}

void Entity::SetBlocked(bool b)
{
	if (b)
	{
		mWorld.GetNavGraph().SetBlock(static_cast<int>(mPosition.x / mWorld.GetNavGraph().GetTileSize()),
			static_cast<int>(mPosition.y / mWorld.GetNavGraph().GetTileSize()));
	}
	else
	{
		mWorld.GetNavGraph().Unblock(static_cast<int>(mPosition.x / mWorld.GetNavGraph().GetTileSize()),
			static_cast<int>(mPosition.y / mWorld.GetNavGraph().GetTileSize()));
	}
}