#include "Precompiled.h"
#include "Constrains.h"

using namespace S;
using namespace S::Physics;

Spring::Spring(Particle * a, Particle * b, float restLength)
	:mParticleA{ a }, mParticleB{ b }, mRestLength{ restLength }
{
	if (mRestLength <= 0.0f)
		mRestLength = Math::Distance(a->position, b->position);
}

void Spring::Apply() const
{
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist*(mParticleA->recipMass + mParticleB->recipMass));
	mParticleA->position += delta * diff*mParticleA->recipMass;
	mParticleB->position -= delta * diff*mParticleB->recipMass;
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(mParticleA->position, mParticleB->position, Math::Vector4::Green());
}
