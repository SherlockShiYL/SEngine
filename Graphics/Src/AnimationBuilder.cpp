#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace S;
using namespace S::Graphics;

namespace
{
	template <class T>
	bool compare(const Keyframe<T> a, const Keyframe<T> b)
	{
		return a.mTime < b.mTime;
	}
}

AnimationBuilder & AnimationBuilder::AddPositionKey(const Math::Vector3 position, const float time)
{
	Keyframe<Math::Vector3> temp = { position,time };
	mAnimation.GetPosKeyFrames().emplace_back(temp);
	mAnimation.GetMaxTime() = mAnimation.GetMaxTime() < time ? time : mAnimation.GetMaxTime();	
	return *this;
}

AnimationBuilder & AnimationBuilder::AddScaleKey(const Math::Vector3 scale, const float time)
{
	Keyframe<Math::Vector3> temp = { scale,time };
	mAnimation.GetScaleKeyFrames().emplace_back(temp);
	mAnimation.GetMaxTime() = mAnimation.GetMaxTime() < time ? time : mAnimation.GetMaxTime();
	return *this;
}

AnimationBuilder & AnimationBuilder::AddRotationkey(const Math::Quaternion quaternion, const float time)
{
	Keyframe<Math::Quaternion> temp = { quaternion,time };
	mAnimation.GetQuatKeyFrames().emplace_back(temp);
	mAnimation.GetMaxTime() = mAnimation.GetMaxTime() < time ? time : mAnimation.GetMaxTime();
	return *this;
}

AnimationBuilder & AnimationBuilder::Addkey(const Math::Vector3 position, const Math::Quaternion quaternion, const float time)
{
	Keyframe<Math::Quaternion> qtemp = { quaternion,time };
	mAnimation.GetQuatKeyFrames().emplace_back(qtemp);
	Keyframe<Math::Vector3> ptemp = { position,time };
	mAnimation.GetPosKeyFrames().emplace_back(ptemp);
	mAnimation.GetMaxTime() = mAnimation.GetMaxTime() < time ? time : mAnimation.GetMaxTime();
	return *this;
}

Animation AnimationBuilder::Finalize()
{
	std::sort(mAnimation.GetPosKeyFrames().begin(), mAnimation.GetPosKeyFrames().end(),compare<Math::Vector3>);
	std::sort(mAnimation.GetScaleKeyFrames().begin(), mAnimation.GetScaleKeyFrames().end(),compare<Math::Vector3>);
	std::sort(mAnimation.GetQuatKeyFrames().begin(), mAnimation.GetQuatKeyFrames().end(),compare<Math::Quaternion>);
	ASSERT(mAnimation.GetPosKeyFrame(0).mTime == 0.0f, "[Animation] First position's time is not 0.0f");
	ASSERT(mAnimation.GetScaleKeyFrame(0).mTime == 0.0f, "[Animation] First scale's time is not 0.0f");
	ASSERT(mAnimation.GetQuatKeyFrame(0).mTime == 0.0f, "[Animation] First rotation's time is not 0.0f");
	return mAnimation;
}
