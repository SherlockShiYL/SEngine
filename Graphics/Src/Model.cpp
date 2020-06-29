#include "Precompiled.h"
#include "Model.h"
#include "ModelLoader.h"

using namespace S;
using namespace S::Graphics;

	/*if (std::string(fileName).rfind(".obj") != std::string::npos)
	{
		meshes.emplace_back(std::make_unique<Mesh>());
		ModelLoader::LoadOBJ(fileName, scale, *meshes.back());
	}
	else
	{
		ModelLoader::LoadSkeleton(fileName, scale, *this);
	}*/

void AnimatedModel::Load(const char * fileName, const wchar_t* shaderfileName)
{
	vertexShader.Initialize(shaderfileName, Graphics::BoneVertex::Format);
	pixelShader.Initialize(shaderfileName);

	ModelLoader::LoadSkeleton(fileName, *this);
	constantBuffer.Initialize(static_cast<uint32_t>(sizeof(Math::Matrix4)*bones.size()));
	
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		meshBuffers.emplace_back();
		meshBuffers.back().Initialize(meshes[i]);
	}
}

void AnimatedModel::Unload()
{
	pixelShader.Terminate();
	vertexShader.Terminate();
	constantBuffer.Terminate();
	for (auto& texture : textures)
		texture.Terminate();
	textures.clear();
	for (auto& mesh : meshes)
		mesh.Destroy();
	meshes.clear();
	for (auto& meshBuffer : meshBuffers)
		meshBuffer.Terminate();
	meshBuffers.clear();
}

void AnimatedModel::Render(const void * data)
{
	vertexShader.Bind();
	pixelShader.Bind();

	constantBuffer.Set(data);
	constantBuffer.BindVS(1);
	for (size_t i = 0; i < meshBuffers.size(); ++i)
	{
		textures[textureIndices[i][0]].BindPS();
		meshBuffers[i].Render();
	}
}

/*void AnimatedModel::GetAnimationMatrices(Bone* bone, float deltaTime, Math::Matrix4 & transform)
{
	animations[0].GetTransform(deltaTime, bone->index, transform);
	if (animations[0].boneAnimations[bone->index] != nullptr)
	{
		test.m[bone->index] = bone->offsetTransform* animations[0].boneAnimations[bone->index]->animation.GetTransform(deltaTime);
	}
	else
	{
		test.m[bone->index] = bone->offsetTransform;
	}
	if (bone->parent != nullptr)
	{
		//test.m[j] = test.m[j] * bone->toParentTransform;
		if (animations[0].boneAnimations[bone->parentIndex] != nullptr)
		{
			test.m[bone->index] = test.m[bone->index] * Math::Inverse(bone->parent->offsetTransform) * test.m[bone->parentIndex];
			//test.m[bone->index] = test.m[bone->index] * animations[0].boneAnimations[bone->parentIndex]->animation.GetTransform(deltaTime);
		}
	}
	for (uint32_t i = 0; i < bone->children.size(); ++i)
	{
		uint32_t te = bone->children[i]->index;
		GetAnimationMatrices(bone->children[i], deltaTime);
	}
}*/

void AnimatedModel::LinkBones()
{
	for (uint32_t i = 0; i < bones.size(); ++i)
	{
		Bone* bone = bones[i];
		if (bone->parentIndex != -1)
		{
			bone->parent = bones[bone->parentIndex];
		}
		else
		{
			root = bone;
		}

		for (uint32_t i = 0; i < bone->childIndices.size(); ++i)
		{
			bone->children[i] = bones[bone->childIndices[i]];
		}
	}
}

void AnimatedModel::PairBoneAnimation()
{
	for (uint32_t i = 0; i < bones.size(); ++i)
	{
		for (uint32_t j = 0; j < animations.size(); ++j)
		{
			for (uint32_t k = i; k < animations[j].boneAnimations.size(); ++k)
			{
				if (animations[j].boneAnimations[k] != nullptr)
				{
					
					if (bones[i]->name == animations[j].boneAnimations[k]->boneName)
					{
						std::swap(animations[j].boneAnimations[k], animations[j].boneAnimations[i]);
						break;
					}
				}
				else
				{
					std::swap(animations[j].boneAnimations[k], animations[j].boneAnimations[i]);
					break;
				}
			}
		}
	}
}
