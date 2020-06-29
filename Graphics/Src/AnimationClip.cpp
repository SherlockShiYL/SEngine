#include "Precompiled.h"
#include "AnimationClip.h"

#include "Bone.h"
#include "BoneAnimation.h"

using namespace S;
using namespace S::Graphics;

bool AnimationClip::GetTransform(float time, uint32_t boneIndex, Math::Matrix4 & transform) const
{
	auto& boneAnim = boneAnimations[boneIndex];
	if (boneAnim)
	{
		transform = boneAnim->animation.GetTransform(time);
		return true;
	}
	return false;
}
