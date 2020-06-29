#ifndef INCLUDED_GRAPHICS_ANIMATIONIO_H
#define INCLUDED_GRAPHICS_ANIMATIONIO_H

namespace S {
namespace Graphics {

class AnimationClip;

class AnimationIO
{
public:
	static void SaveAnimationClip(FILE* file, const AnimationClip& animClip);
	static void LoadAnimationClip(FILE* file, std::vector<AnimationClip>& animClip, uint32_t numBones);
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_ANIMATIONIO_H