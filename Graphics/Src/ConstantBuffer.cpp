#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace S;
using namespace S::Graphics;

void ConstantBuffer::Initialize(uint32_t bufferSize, const void* initData)
{
	// Create the constant buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer);
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void ConstantBuffer::Set(const void * data)
{
	GraphicsSystem::Get()->GetContext()->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindGS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->GSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}

void ConstantBuffer::BindCS(uint32_t slot)
{
	GraphicsSystem::Get()->GetContext()->CSSetConstantBuffers(slot, 1, &mConstantBuffer);
}