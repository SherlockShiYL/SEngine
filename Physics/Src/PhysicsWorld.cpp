#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "Particle.h"

using namespace S;
using namespace S::Physics;

void PhysicsWorld::Initialize(const Settings & settings)
{
	mSettings = settings;
}

void PhysicsWorld::Update(float deltaTime)
{
	if (!mPause)
	{
		mTimer += deltaTime;
		while (mTimer >= mSettings.timeStep)
		{
			mTimer -= mSettings.timeStep;
			AccumulateForces();
			Integrate();
			SatisfyConstraints();
		}
	}
}

void PhysicsWorld::DebugDraw() const
{
	for (auto p : mParticles)
		Graphics::SimpleDraw::AddSphere(p->position, p->radius, Math::Vector4::Black(), 4, 2);
	for (auto c : mConstraints)
		c->DebugDraw();
	for (auto& obb : mOBBs)
		Graphics::SimpleDraw::AddOBB(obb, Math::Vector4::Cyan());
}

void PhysicsWorld::AddParticle(Particle * p)
{
	mParticles.push_back(p);
}

void PhysicsWorld::AddConstraint(Constraint * c)
{
	mConstraints.push_back(c);
}

void PhysicsWorld::AddPlane(const Geometry::Plane& plane)
{
	mPlanes.push_back(plane);
}

void PhysicsWorld::AddOBB(const Geometry::OBB & obb)
{
	mOBBs.push_back(obb);
}

void PhysicsWorld::ClearDynamic()
{
	for (auto particle : mParticles)
		SafeDelete(particle);
	mParticles.clear();

	for (auto c : mConstraints)
		SafeDelete(c);
	mConstraints.clear();
}

void PhysicsWorld::Pause(bool pause)
{
	mPause = pause;
}

void PhysicsWorld::AccumulateForces()
{
	for (auto p : mParticles)
		p->acceleration = mSettings.gravity;
}

void PhysicsWorld::Integrate()
{
	const float gamma = 1.0f - mSettings.drag;
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto p : mParticles)
	{
		if (!p->sleep)
		{
			Math::Vector3 displacement = (p->position - p->lastPosition)*gamma + (p->acceleration*timeStepSqr);
			if (displacement.z < 0.0f)
			{
				displacement.z *= 0.96f;
			}
			p->lastPosition = p->position;
			p->position = p->position + displacement;
		}
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	for (uint32_t n = 0; n < mSettings.iterations; ++n)
	{
		for (auto c : mConstraints)
			c->Apply();
	}

	for (auto plane : mPlanes)
	{
		for (auto p : mParticles)
		{
			if (Math::Dot(p->position, plane.n) <= plane.d &&
				Math::Dot(p->lastPosition, plane.n) > plane.d)
			{
				// New Velocity is wrong
				Math::Vector3 velocity = p->position - p->lastPosition;
				Math::Vector3 velocityPerpendicular = plane.n * Math::Dot(velocity, plane.n);
				Math::Vector3 velocityParallel = velocity - velocityPerpendicular;
				Math::Vector3 newVelocity = velocityParallel * (1.0f - mSettings.drag) - velocityPerpendicular * p->bounce;
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}
		}
	}

	for (auto obb : mOBBs)
	{
		for (auto p : mParticles)
		{
			if (Geometry::Intersect(p->position, obb))
			{
				Math::Vector3 velocity = p->position - p->lastPosition;
				if (!Math::Compare(velocity, Math::Vector3::Zero()))
				{
					Math::Vector3 direction = Math::Normalize(velocity);

					Geometry::Ray ray{ p->lastPosition,direction };
					Math::Vector3 point, normal;
					Geometry::GetContactPoint(ray, obb, point, normal);

					Math::Vector3 velocityPerpendicular = normal * Math::Dot(velocity, normal);
					Math::Vector3 velocityParallel = velocity - velocityPerpendicular;
					Math::Vector3 newVelocity = velocityParallel * (1.0f - mSettings.drag) - velocityPerpendicular * p->bounce;
					p->SetPosition(p->position - velocityPerpendicular);
					p->SetVelocity(newVelocity);
				}
			}
		}
	}
}
