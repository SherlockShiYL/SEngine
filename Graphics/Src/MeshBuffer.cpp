#include "Precompiled.h"
#include "MeshBuffer.h"

#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace S;
using namespace S::Graphics;

MeshBuffer::~MeshBuffer()
{
	ASSERT(mVertexBuffer == nullptr, "[MeshBuffer] Vertex buffer not released!");
	ASSERT(mIndexBuffer == nullptr, "[MeshBuffer] Index buffer not released!");
}

MeshBuffer::MeshBuffer(MeshBuffer && rhs) noexcept
	: mVertexBuffer{ std::exchange(rhs.mVertexBuffer,nullptr) }
	, mIndexBuffer{ std::exchange(rhs.mIndexBuffer,nullptr) }
	, mVertexSize{ std::exchange(rhs.mVertexSize,0) }
	, mVertexCount{ std::exchange(rhs.mVertexCount,0) }
	, mMaxVertexCount{ std::exchange(rhs.mMaxVertexCount,0) }
	, mIndexCount{ std::exchange(rhs.mIndexCount,0) }
	, mTopology{ std::exchange(rhs.mTopology,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) }
	, mDynamic{ std::exchange(rhs.mDynamic,false) }
{
}

MeshBuffer & MeshBuffer::operator=(MeshBuffer && rhs) noexcept
{
	MeshBuffer{ std::move(rhs) }.Swap(*this);
	return *this;
}

void MeshBuffer::Swap(MeshBuffer & rhs) noexcept
{
	std::swap(mVertexBuffer, rhs.mVertexBuffer);
	std::swap(mIndexBuffer, rhs.mIndexBuffer);
	std::swap(mTopology, rhs.mTopology);
	std::swap(mVertexSize, rhs.mVertexSize);
	std::swap(mVertexCount, rhs.mVertexCount);
	std::swap(mMaxVertexCount, rhs.mMaxVertexCount);
	std::swap(mIndexCount, rhs.mIndexCount);
	std::swap(mDynamic, rhs.mDynamic);
}

void MeshBuffer::Initialize(const Mesh& mesh, bool dynamic)
{
	Initialize(mesh.GetVertices(), sizeof(Vertex), mesh.GetVertexCount(), mesh.GetIndices(), mesh.GetIndexCount(), dynamic);
}

void MeshBuffer::Initialize(const MeshPT& mesh, bool dynamic)
{
	Initialize(mesh.GetVertices(), sizeof(VertexPT), mesh.GetVertexCount(), mesh.GetIndices(), mesh.GetIndexCount(), dynamic);
}

void MeshBuffer::Initialize(const BoneMesh & mesh, bool dynamic)
{
	Initialize(mesh.GetVertices(), sizeof(BoneVertex), mesh.GetVertexCount(), mesh.GetIndices(), mesh.GetIndexCount(), dynamic);
}

void MeshBuffer::Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, bool dynamic)
{
	CreateVertexBuffer(vertexData, vertexSize, numVertices, dynamic);
}

void MeshBuffer::Initialize(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, const uint32_t* indexData, uint32_t numIndices, bool dynamic)
{
	CreateVertexBuffer(vertexData, vertexSize, numVertices, dynamic);
	CreateIndexBuffer(indexData, numIndices);
}

void MeshBuffer::CreateVertexBuffer(const void* vertexData, uint32_t vertexSize, uint32_t numVertices, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = numVertices;
	mMaxVertexCount = numVertices;
	mDynamic = dynamic;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexSize * mVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertexData;

	GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, vertexData ? &initData : nullptr, &mVertexBuffer);
}

void MeshBuffer::CreateIndexBuffer(const uint32_t* indexData, uint32_t numIndices)
{
	mIndexCount = numIndices;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t) * mIndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indexData;

	GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, &initData, &mIndexBuffer);
}

void MeshBuffer::Initialize(int verticesize, const void* vertices, int verticescount, const uint32_t* index, int indexcount)
{
	mVertexCount = verticescount;
	mVertexSize = verticesize;

	// FIll our buffer description so D3D knows how to create the vertex buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = static_cast<UINT>(mVertexSize*mVertexCount);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Initialize our vertex buffer by feeding it our triangle vertices
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	// Create the vertex buffer using the D3D device
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);

	mIndexCount = indexcount;

	// Now fill in our index buffer description
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(uint32_t)*mIndexCount);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Initialize with our index data
	initData.pSysMem = index;

	// Create the index buffer using the D3D device
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
}

void MeshBuffer::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mIndexBuffer);
}

void MeshBuffer::UpdateVertices(const void* vertexData, uint32_t numVertices)
{
	ASSERT(mDynamic, "[MeshBuffer] Vertex buffer must be dynamic to call UpdateBuffer()!");
	ASSERT(numVertices <= mMaxVertexCount, "[MeshBuffer] Vertex data size mismatch!");

	mVertexCount = numVertices;

	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertexData, numVertices * mVertexSize);
	context->Unmap(mVertexBuffer, 0);
}

void MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case Topology::LineList:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case Topology::TriangleList:
		mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	}
}

void MeshBuffer::Render() const
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	// Connect our vertex buffer
	UINT stride = mVertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Check if we are using index buffer
	if (mIndexBuffer != nullptr)
	{
		// Set index buffer
		context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Draw indexed mesh
		context->DrawIndexed(mIndexCount, 0, 0);
	}
	else
	{
		// Draw mesh
		context->Draw(mVertexCount, 0);
	}
}
