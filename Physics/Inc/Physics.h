#ifndef INCLUDED_PHYSICS_H
#define INCLUDED_PHYSICS_H

#include "Common.h"

#include "ColliderManager.h"
#include "Constrains.h"
#include "Particle.h"
#include "PhysicsWorld.h"

namespace S {
namespace Physics {

	void AddCollider(CircleCollider cCollider, uint32_t type);

}
}

#endif // !INCLUDED_PHYSICS_H