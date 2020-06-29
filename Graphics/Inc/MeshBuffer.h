#ifndef INCLUDED_GRAPHICS_MESHBUFFER_H
#define INCLUDED_GRAPHICS_MESHBUFFER_H

#include "Mesh.h"

namespace S {
namespace Graphics {

class MeshBuffer
{
public:
	enum class Topology
	{
		LineList,
		TriangleList
	};

	MeshBuffer() = default;
	~MeshBuffer();

	MeshBuffer(const MeshBuffer&) = delete;
	MeshBuffer& operator=(const MeshBuffer&) = delete;

	MeshBuffer(MeshBuffer&& rhs) noexcept;
	MeshBuffer& operator=(MeshBuffer&& rhs) noexcept;

	void Swap(MeshBuffer& rhs) noexcept;

	void Initialize(const Mesh& mesh, bool dynamic = false);
	void Initialize(const MeshPT& mesh, bool dynamic = false);
	void Initialize(const BoneMesh& mesh, bool dynamic = false);
	void Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, bool dynamic = false);
	void Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, const uint32_t* indexData, uint32_t numIndices, bool dynamic = false);
	void Initialize(int verticesize, const void* vertices, int verticescount, const uint32_t* index, int indexcount);
	void Terminate();

	void SetTopology(Topology topology);

	void UpdateVertices(const void* vertexData, uint32_t numVertices);

	void Render() const;

private:
	void CreateVertexBuffer(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, bool dynamic);
	void CreateIndexBuffer(const uint32_t* indexData, uint32_t numIndices);

	ID3D11Buffer* mVertexBuffer{ nullptr };
	ID3D11Buffer* mIndexBuffer{ nullptr };

	D3D11_PRIMITIVE_TOPOLOGY mTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST };

	size_t mVertexSize{ 0 };
	size_t mVertexCount{ 0 };
	size_t mMaxVertexCount{ 0 };
	size_t mIndexCount{ 0 };

	bool mDynamic{ false };

};

} // namepsace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_MESHBUFFER_H