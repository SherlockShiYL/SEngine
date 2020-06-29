#ifndef INCLUDED_GRAPHICS_MESH_H
#define INCLUDED_GRAPHICS_MESH_H

#include "VertexTypes.h"

namespace S {
namespace Graphics {

template <class VertexType>
class BaseMesh
{
public:
	void Allocate(uint32_t numVertices, uint32_t numIndices)
	{
		mVertices = std::make_unique<VertexType[]>(numVertices);
		mIndices = std::make_unique<uint32_t[]>(numIndices);
		mNumVertices = numVertices;
		mNumIndices = numIndices;
	}

	void Destroy()
	{
		mVertices.reset();
		mIndices.reset();
		mNumVertices = 0;
		mNumIndices = 0;
	}

	VertexType& GetVertex(uint32_t index) const
	{
		ASSERT(index < mNumVertices, "[Mesh] Index out of range.");
		return mVertices[index];
	}
	uint32_t& GetIndex(uint32_t index) const
	{
		ASSERT(index < mNumIndices, "[Mesh] Index out of range.");
		return mIndices[index];
	}

	const VertexType* GetVertices() const	{ return mVertices.get(); }
	const uint32_t* GetIndices() const		{ return mIndices.get(); }
	uint32_t GetVertexCount() const			{ return mNumVertices; }
	uint32_t GetIndexCount() const			{ return mNumIndices; }

private:
	std::unique_ptr<VertexType[]> mVertices;
	std::unique_ptr<uint32_t[]> mIndices;

	uint32_t mNumVertices{ 0 };
	uint32_t mNumIndices{ 0 };

};

using MeshP = BaseMesh<VertexP>;
using MeshPC = BaseMesh<VertexPC>;
using MeshPNC = BaseMesh<VertexPNC>;
using MeshPT = BaseMesh<VertexPT>;
using MeshPNT = BaseMesh<VertexPNT>;
using Mesh = BaseMesh<Vertex>;
using BoneMesh = BaseMesh<BoneVertex>;

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_MESH_H