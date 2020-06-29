#ifndef INCLUDED_PHYSICS_PARTICLE_H
#define INCLUDED_PHYSICS_PARTICLE_H

namespace S {
namespace Physics {

struct Particle
{
	Math::Vector3 position{ 0.0f };
	Math::Vector3 lastPosition{ 0.0f };
	Math::Vector3 acceleration{ 0.0f };

	float radius{ 1.0f };
	float recipMass{ 0.0f };
	float bounce{ 0.0f };
	bool sleep{ true };

	void SetPosition(const Math::Vector3& pos)
	{
		// position = lastPosition = no motion
		position = pos;
		lastPosition = pos;
	}

	void SetVelocity(const Math::Vector3& velocity)
	{
		lastPosition = position - velocity;
		sleep = false;
	}

};

} // namespace Physics
} // namespace S

#endif // !INCLUDED_PHYSICS_PARTICLE_H