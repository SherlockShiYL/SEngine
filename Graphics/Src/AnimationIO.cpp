#include "Precompiled.h"
#include "AnimationIO.h"
#include "AnimationClip.h"
#include "AnimationBuilder.h"

using namespace S;
using namespace S::Graphics;

void AnimationIO::SaveAnimationClip(FILE * file, const AnimationClip & animClip)
{
	fprintf(file, "AnimationName: %s\n", animClip.name.c_str());
	fprintf(file, "Duration: %f\n", animClip.duration);
	fprintf(file, "TicksPerSecond: %f\n", animClip.ticksPerSecond);
	for (uint32_t i = 0; i < animClip.boneAnimations.size(); ++i)
	{
		fprintf(file, "BoneName: %s\n", animClip.boneAnimations[i]->boneName.c_str());
		fprintf(file, "PosKeyframes: %u\n", animClip.boneAnimations[i]->animation.mPosKeyframes.size());
		for (uint32_t index = 0; index < animClip.boneAnimations[i]->animation.mPosKeyframes.size(); ++index)
		{
			fprintf(file, "%f %f %f %f\n", animClip.boneAnimations[i]->animation.mPosKeyframes[index].mTime
				, animClip.boneAnimations[i]->animation.mPosKeyframes[index].mValue.x
				, animClip.boneAnimations[i]->animation.mPosKeyframes[index].mValue.y
				, animClip.boneAnimations[i]->animation.mPosKeyframes[index].mValue.z);
		}
		fprintf(file, "QuatKeyframes: %u\n", animClip.boneAnimations[i]->animation.mQuatKeyframes.size());
		for (uint32_t index = 0; index < animClip.boneAnimations[i]->animation.mQuatKeyframes.size(); ++index)
		{
			fprintf(file, "%f %f %f %f %f\n", animClip.boneAnimations[i]->animation.mQuatKeyframes[index].mTime
				, animClip.boneAnimations[i]->animation.mQuatKeyframes[index].mValue.x
				, animClip.boneAnimations[i]->animation.mQuatKeyframes[index].mValue.y
				, animClip.boneAnimations[i]->animation.mQuatKeyframes[index].mValue.z
				, animClip.boneAnimations[i]->animation.mQuatKeyframes[index].mValue.w);
		}
		fprintf(file, "ScaleKeyframes: %u\n", animClip.boneAnimations[i]->animation.mScaleKeyframes.size());
		for (uint32_t index = 0; index < animClip.boneAnimations[i]->animation.mScaleKeyframes.size(); ++index)
		{
			fprintf(file, "%f %f %f %f\n", animClip.boneAnimations[i]->animation.mScaleKeyframes[index].mTime
				, animClip.boneAnimations[i]->animation.mScaleKeyframes[index].mValue.x
				, animClip.boneAnimations[i]->animation.mScaleKeyframes[index].mValue.y
				, animClip.boneAnimations[i]->animation.mScaleKeyframes[index].mValue.z);
		}
	}
}

void AnimationIO::LoadAnimationClip(FILE * file, std::vector<AnimationClip> & animClip, uint32_t numBones)
{
	int animIndex{ -1 };
	uint32_t boneIndex{ 0 };
	while (true)
	{
		char buffer[256];
		int res = fscanf_s(file, "%s", buffer, std::size(buffer));

		if (res == EOF)
			break;
		if (strcmp(buffer, "AnimationCount:") == 0)
		{
			uint32_t num;
			fscanf_s(file, "%u\n", &num);
			animClip.resize(num);
		}
		else if (strcmp(buffer, "AnimationName:") == 0)
		{
			fscanf_s(file, "%s\n", buffer, std::size(buffer));
			animClip[++animIndex].name = std::string(buffer);
			animClip[animIndex].boneAnimations.resize(numBones);
			boneIndex = 0u;
		}
		else if (strcmp(buffer, "Duration:") == 0)
		{
			fscanf_s(file, "%f\n", &animClip[animIndex].duration);
		}
		else if (strcmp(buffer, "TicksPerSecond:") == 0)
		{
			fscanf_s(file, "%f\n", &animClip[animIndex].ticksPerSecond);
		}
		else if (strcmp(buffer, "BoneName:") == 0)
		{
			fscanf_s(file, "%s\n", buffer, std::size(buffer));
			animClip[animIndex].boneAnimations[boneIndex] = std::make_unique<BoneAnimation>();
			animClip[animIndex].boneAnimations[boneIndex]->boneName += buffer;
			AnimationBuilder builder;
			for (int i = 0; i < 3; ++i)
			{
				fscanf_s(file, "%s", buffer, std::size(buffer));
				if (strcmp(buffer, "PosKeyframes:") == 0)
				{
					uint32_t num;
					fscanf_s(file, "%u\n", &num);
					for (uint32_t i = 0; i < num; ++i)
					{
						Math::Vector3 v;
						float t;
						fgets(buffer, std::size(buffer), file);
						if (sscanf_s(buffer, "%f %f %f %f",
							&t, &v.x, &v.y, &v.z) != 4)
						{
							ASSERT(false, "[AnimationIO] Failed to load Position in Model file.");
						}
						else
						{
							builder.AddPositionKey(v, t / animClip[animIndex].ticksPerSecond);
						}
					}
				}
				else if (strcmp(buffer, "QuatKeyframes:") == 0)
				{
					uint32_t num;
					fscanf_s(file, "%u\n", &num);
					for (uint32_t i = 0; i < num; ++i)
					{
						Math::Quaternion q;
						float t;
						fgets(buffer, std::size(buffer), file);
						if (sscanf_s(buffer, "%f %f %f %f %f", &t, &q.x, &q.y, &q.z, &q.w) != 5)
						{
							ASSERT(false, "[AnimationIO] Failed to load Rotation in Model file.");
						}
						else
						{
							builder.AddRotationkey(q, t / animClip[animIndex].ticksPerSecond);
						}
					}
				}
				else if (strcmp(buffer, "ScaleKeyframes:") == 0)
				{
					uint32_t num;
					fscanf_s(file, "%u\n", &num);
					for (uint32_t i = 0; i < num; ++i)
					{
						Math::Vector3 v;
						float t;
						fgets(buffer, std::size(buffer), file);
						if (sscanf_s(buffer, "%f %f %f %f", &t, &v.x, &v.y, &v.z) != 4)
						{
							ASSERT(false, "[AnimationIO] Failed to load Scale in Model file.");
						}
						else
						{
							builder.AddScaleKey(v, t / animClip[animIndex].ticksPerSecond);
						}
					}
				}
			}
			animClip[animIndex].boneAnimations[boneIndex++]->animation = builder.Finalize();
		}
	}
}