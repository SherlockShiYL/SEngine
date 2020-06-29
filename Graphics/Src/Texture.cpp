#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <External/DirectXTK/Inc/DDSTextureLoader.h>
#include <External/DirectXTK/Inc/WICTextureLoader.h>
#include <wrl/client.h>

using namespace S;
using namespace S::Graphics;

Texture::Texture()
	: mShaderResourceView(nullptr)
{
}

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "[Texture] Texture not released!");
}

bool Texture::Initialize(const char* fileName)
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();

	wchar_t wideFileName[1024];
	mbstowcs_s(nullptr, wideFileName, fileName, 1024);

	if (wcsstr(wideFileName, L".dds") != nullptr)
	{
		DirectX::CreateDDSTextureFromFile(device, context, wideFileName, nullptr, &mShaderResourceView);
	}
	else
	{
		DirectX::CreateWICTextureFromFile(device, context, wideFileName, nullptr, &mShaderResourceView);
	}

	if (mShaderResourceView)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		mShaderResourceView->GetResource(&resource);

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
		resource.As(&texture2D);

		texture2D->GetDesc(&mTextureDescription);
	}

	return mShaderResourceView != nullptr;
}

void Texture::Initialize(const void* data, uint32_t width, uint32_t height)
{
	mTextureDescription.Width = width;
	mTextureDescription.Height = height;
	mTextureDescription.MipLevels = 1;
	mTextureDescription.ArraySize = 1;
	mTextureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	mTextureDescription.SampleDesc.Count = 1;
	mTextureDescription.SampleDesc.Quality = 0;
	mTextureDescription.Usage = D3D11_USAGE_DYNAMIC;
	mTextureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	mTextureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mTextureDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;
	initData.SysMemPitch = width * 4;

	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&mTextureDescription, &initData, &texture);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to create texture.");

	hr = device->CreateShaderResourceView(texture, nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to create shader resource view.");

	SafeRelease(texture);
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::Set(const void* data)
{
	ASSERT(mTextureDescription.Usage == D3D11_USAGE_DYNAMIC, "[Texture] Cannot update data for static texture.");
	
	Microsoft::WRL::ComPtr<ID3D11Resource> resourceInterface;
	mShaderResourceView->GetResource(&resourceInterface);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
	resourceInterface.As(&texture2D);

	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();

	D3D11_MAPPED_SUBRESOURCE resource;
	context->Map(texture2D.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	const uint8_t* src = static_cast<const uint8_t*>(data);
	uint8_t* dst = static_cast<uint8_t*>(resource.pData);
	for (uint32_t i = 0; i < mTextureDescription.Height; ++i)
	{
		memcpy(dst, src, mTextureDescription.Width * 4);
		src += mTextureDescription.Width * 4;
		dst += resource.RowPitch;
	}
	context->Unmap(texture2D.Get(), 0);
}

void Texture::BindVS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

Texture::Texture(Texture&& rhs) noexcept
	:mShaderResourceView{ std::exchange(rhs.mShaderResourceView,nullptr) }
	, mTextureDescription{ std::exchange(rhs.mTextureDescription, D3D11_TEXTURE2D_DESC{}) }
{
}

Texture & Texture::operator=(Texture && rhs) noexcept
{
	Texture{ std::move(rhs) }.Swap(*this);
	return *this;
}

void Texture::Swap(Texture & rhs) noexcept
{
	std::swap(mShaderResourceView, rhs.mShaderResourceView);
	std::swap(mTextureDescription, rhs.mTextureDescription);
}