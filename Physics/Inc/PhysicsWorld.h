#ifndef INCLUDED_PHYSICS_PHYSICSWORLD_H
#define INCLUDED_PHYSICS_PHYSICSWORLD_H

#include "Constrains.h"

namespace S {
namespace Physics {

struct Particle;

struct Settings
{
	Math::Vector3 gravity{ 0.0f,-9.8f,0.0f };
	float timeStep{ 1.0f / 60.0f };
	float drag{ 0.0f };
	uint32_t iterations{ 1 };
};

class PhysicsWorld
{
public:
	void Initialize(const Settings& settings);

	void Update(float deltaTime);
	void DebugDraw() const;

	void AddParticle(Particle* p);
	void AddConstraint(Constraint* c);
	void AddPlane(const Geometry::Plane& plane);
	void AddOBB(const Geometry::OBB& obb);

	void ClearDynamic();
	void Pause(bool pause);

	std::vector<Particle*>& GetParticles()					{ return mParticles; }
	const std::vector<Particle*>& GetParticles() const		{ return mParticles; }

	std::vector<Geometry::OBB>& GetOBBs()					{ return mOBBs; }
	const std::vector<Geometry::OBB>& GetOBBs() const		{ return mOBBs; }

private:
	void AccumulateForces();
	void Integrate();
	void SatisfyConstraints();

	std::vector<Particle*> mParticles;
	std::vector<Constraint*> mConstraints;
	std::vector<Geometry::Plane> mPlanes;
	std::vector<Geometry::OBB> mOBBs;
	Settings mSettings;
	float mTimer{ 0.0f };
	bool mPause{ false };
};

} // namespace Physics
} // namespace S

#endif // !INCLUDED_PHYSICS_PHYSICSWORLD_H