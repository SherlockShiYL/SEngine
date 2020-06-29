#ifndef INCLUDED_GRAPHICS_VERTEXTYPES_H
#define INCLUDED_GRAPHICS_VERTEXTYPES_H

#include "Common.h"

namespace S {
namespace Graphics {

// Define a C++ structure for our vertex, this however, is not
// something the GPU understands.

// New struct need to be added into VertexShader.cpp & Mesh.h & MeshBuilder.h
struct VertexP
{
	static constexpr int Format = 0;

	Math::Vector3 position;
};

struct VertexPC
{
	static constexpr int Format = 1;

	Math::Vector3 position;
	Math::Vector4 color;
};

struct VertexPNC
{
	static constexpr int Format = 2;

	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector4 color;
};

struct VertexPT
{
	static constexpr int Format = 3;

	Math::Vector3 position;
	Math::Vector2 texcoord;
};

struct VertexPNT
{
	static constexpr int Format = 4;

	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector2 texcoord;};

struct Vertex
{
	static constexpr int Format = 5;

	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector2 texcoord;};

struct BoneVertex
{
	static constexpr int Format = 6;

	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector3 tangent;
	Math::Vector2 texcoord;	int boneIndices[4]{ 0 };	float boneWeight[4]{ 0.0f };};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_VERTEXTYPES_H