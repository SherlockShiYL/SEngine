#ifndef INCLUDED_GRAPHICS_ANIMATION_H
#define INCLUDED_GRAPHICS_ANIMATION_H

#include "Keyframe.h"

namespace S {
namespace Graphics {

class Animation
{
public:
	Math::Matrix4 GetTransform(float deltaTime);

	float& GetMaxTime() { return mMaxTime; }

	std::vector<Keyframe<S::Math::Quaternion>>& GetQuatKeyFrames()
	{
		return mQuatKeyframes;
	}
	std::vector<Keyframe<S::Math::Vector3>>& GetPosKeyFrames()
	{
		return mPosKeyframes;
	}
	std::vector<Keyframe<S::Math::Vector3>>& GetScaleKeyFrames()
	{
		return mScaleKeyframes;
	}

	Keyframe<S::Math::Quaternion>& GetQuatKeyFrame(uint32_t index)
	{
		return mQuatKeyframes[index];
	}

	Keyframe<S::Math::Vector3>& GetPosKeyFrame(uint32_t index)
	{
		return mPosKeyframes[index];
	}

	Keyframe<S::Math::Vector3>& GetScaleKeyFrame(uint32_t index)
	{
		return mScaleKeyframes[index];
	}

private:
	friend class AnimationBuilder;
	friend class AnimationIO;

	std::vector<Keyframe<S::Math::Quaternion>> mQuatKeyframes;
	std::vector<Keyframe<S::Math::Vector3>> mPosKeyframes;
	std::vector<Keyframe<S::Math::Vector3>> mScaleKeyframes;
	float mMaxTime{ 0.0f };
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_ANIMATION_H