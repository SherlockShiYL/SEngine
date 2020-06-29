#ifndef INCLUDED_GRAPHICS_MODEL_H
#define INCLUDED_GRAPHICS_MODEL_H

#include "AnimationClip.h"
#include "Bone.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "Texture.h"
#include "PixelShader.h"
#include "VertexShader.h"

namespace S {
namespace Graphics {

struct Model
{
	std::vector<Mesh> meshes;
};

struct AnimatedModel
{
	void Load(const char* fileName, const wchar_t* shaderfileName);
	void Unload();
	void Render(const void * data);

	void LinkBones();
	void PairBoneAnimation();

	Bone* root{ nullptr };
	std::vector<Bone*> bones;
	std::vector<Texture> textures;
	std::vector<std::vector<int>> textureIndices;
	std::vector<AnimationClip> animations;
	std::vector<BoneMesh> meshes;
	std::vector<MeshBuffer> meshBuffers;
	VertexShader vertexShader;
	PixelShader pixelShader;
	ConstantBuffer constantBuffer;
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_MODEL_H