#include "Precompiled.h"
#include "TransformComponent.h"

using namespace S;

META_CLASS_BEGIN(TransformComponent)
META_FIELD_BEGIN
META_FIELD(mPosition, "Position")
META_FIELD(mRotation, "Rotation")
META_FIELD(mScale, "Scale")
META_FIELD_END
META_CLASS_END

TransformComponent::TransformComponent()
	: Component(StaticGetTypedId())
{
}

void TransformComponent::Initialize()
{
}

void TransformComponent::Terminate()
{
}

void TransformComponent::Update(float deltatTime)
{
}

void TransformComponent::Render()
{
}

void TransformComponent::Render2D()
{
}
