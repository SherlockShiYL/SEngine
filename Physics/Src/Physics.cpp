#include "Precompiled.h"
#include "Physics.h"

namespace S {
namespace Physics {

void AddCollider(CircleCollider cCollider, uint32_t type)
{
	ColliderManager::Get()->AddCollider(cCollider, type);
}

}
}