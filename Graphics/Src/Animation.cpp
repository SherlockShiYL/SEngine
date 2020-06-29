#include "Precompiled.h"
#include "Animation.h"

using namespace S;
using namespace S::Math;
using namespace S::Graphics;

Matrix4 Animation::GetTransform(float deltaTime)
{
	if (mMaxTime != 0.0f)
	{
		while (deltaTime > mMaxTime)
		{
			deltaTime -= mMaxTime;
		}
	}
	// Position
	Vector3 position;
	uint32_t num = mPosKeyframes.size();
	if (num > 1)
	{
		if (deltaTime < mPosKeyframes[mPosKeyframes.size() - 1].mTime)
		{
			for (uint32_t i = 0; i < num - 1; ++i)
			{
				/*if (time > mPosKeyframes[num - 1].mTime)
				{
					time -= static_cast<float>(static_cast<int>(time / mPosKeyframes[num - 1].mTime))*mPosKeyframes[num - 1].mTime;
				}*/
				if (deltaTime >= mPosKeyframes[i].mTime && deltaTime < mPosKeyframes[i + 1].mTime)
				{
					float timeGap = mPosKeyframes[i + 1].mTime - mPosKeyframes[i].mTime;
					float rate = (deltaTime - mPosKeyframes[i].mTime) / timeGap;
					position = mPosKeyframes[i].mValue + (mPosKeyframes[i + 1].mValue - mPosKeyframes[i].mValue) * rate;
					break;
				}
			}
		}
		else
		{
			position = mPosKeyframes[mPosKeyframes.size() - 1].mValue;
		}
	}
	else
	{
		position = mPosKeyframes[0].mValue;
	}

	// Scale
	Vector3 scale;
	num = mScaleKeyframes.size();
	if (num > 1)
	{
		if (deltaTime < mScaleKeyframes[mScaleKeyframes.size() - 1].mTime)
		{
			for (uint32_t i = 0; i < num - 1; ++i)
			{
				/*if (time > mScaleKeyframes[num - 1].mTime)
				{
					time -= static_cast<int>(static_cast<int>(time / mScaleKeyframes[num - 1].mTime))*mScaleKeyframes[num - 1].mTime;
				}*/
				if (deltaTime >= mScaleKeyframes[i].mTime && deltaTime < mScaleKeyframes[i + 1].mTime)
				{
					float timeGap = mScaleKeyframes[i + 1].mTime - mScaleKeyframes[i].mTime;
					float rate = (deltaTime - mScaleKeyframes[i].mTime) / timeGap;
					scale = mScaleKeyframes[i].mValue + (mScaleKeyframes[i + 1].mValue - mScaleKeyframes[i].mValue) * rate;
					break;
				}
			}
		}
		else
		{
			scale = mScaleKeyframes[mScaleKeyframes.size() - 1].mValue;
		}
	}
	else
	{
		scale = mScaleKeyframes[0].mValue;
	}

	// Rotation
	Quaternion rotation;
	num = mQuatKeyframes.size();
	if (num > 1)
	{
		if (deltaTime < mQuatKeyframes[mQuatKeyframes.size() - 1].mTime)
		{
			for (uint32_t i = 0; i < num - 1; ++i)
			{
				/*if (time > mQuatKeyframes[num - 1].mTime)
				{
					time -= static_cast<int>(static_cast<int>(time / mQuatKeyframes[num - 1].mTime))*mQuatKeyframes[num - 1].mTime;
				}*/
				if (deltaTime >= mQuatKeyframes[i].mTime && deltaTime < mQuatKeyframes[i + 1].mTime)
				{
					float timeGap = mQuatKeyframes[i + 1].mTime - mQuatKeyframes[i].mTime;
					float rate = (deltaTime - mQuatKeyframes[i].mTime) / timeGap;
					rotation = Slerp(mQuatKeyframes[i].mValue, mQuatKeyframes[i + 1].mValue, rate);
					break;
				}
			}
		}
		else
		{
			rotation = mQuatKeyframes[mQuatKeyframes.size() - 1].mValue;
		}
	}
	else
	{
		rotation = mQuatKeyframes[0].mValue;
	}

	return MatrixRotationQuaternion(rotation)*Scaling(scale)*Translation(position);
}
