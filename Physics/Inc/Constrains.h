#ifndef INCLUDED_PHYSICS_CONSTRAINTS_H
#define INCLUDED_PHYSICS_CONSTRAINTS_H

#include "Particle.h"

namespace S {
namespace Physics{

class Constraint
{
public:
	virtual ~Constraint() = default;
	virtual void Apply() const = 0;
	virtual void DebugDraw() const {}
};

class Spring :public Constraint
{
public:
	Spring(Particle* a, Particle* b, float restLength = 0.0f);

	void Apply() const override;
	void DebugDraw() const override;

private:
	Particle* mParticleA{ nullptr };
	Particle* mParticleB{ nullptr };
	float mRestLength{ 1.0f };
};

} // namespace Physics
} // namespace S

#endif // !INCLUDED_PHYSICS_CONSTRAINTS_H