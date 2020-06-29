#ifndef INCLUDED_GRAPHICS_BONEANIMATION_H
#define INCLUDED_GRAPHICS_BONEANIMATION_H

#include "Animation.h"

namespace S {
namespace Graphics {

struct Bone;

class BoneAnimation : public Animation
{
public:
	std::string boneName;
	Animation animation;
};

using BoneAnimations = std::vector<std::unique_ptr<BoneAnimation>>;

} // namespace Graphics
} // namepsace S

#endif // !INCLUDED_GRAPHICS_BONEANIMATION_H