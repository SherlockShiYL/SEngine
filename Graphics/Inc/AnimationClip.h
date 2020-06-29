#ifndef INCLUDED_GRAPHICS_ANIMATIONCLIP_H
#define INCLUDED_GRAPHICS_ANIMATIONCLIP_H

#include "BoneAnimation.h"

namespace S {
namespace Graphics {

class AnimationClip
{
public:
	AnimationClip() = default;

	bool GetTransform(float time, uint32_t boneIndex, Math::Matrix4& transform) const;

	BoneAnimations boneAnimations;
	std::string name;
	float duration{ 0.0f };
	float ticksPerSecond{ 0.0f };
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_ANIMATIONCLIP_H