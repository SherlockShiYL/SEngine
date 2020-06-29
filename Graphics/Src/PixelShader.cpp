#include "Precompiled.h"
#include "PixelShader.h"

#include "GraphicsSystem.h"

using namespace S;
using namespace S::Graphics;

void PixelShader::Initialize(const wchar_t* fileName)
{
	// Compile the pixel shader code
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	D3DCompileFromFile(
		fileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_4_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);

	// Create pixel shader
	Graphics::GraphicsSystem::Get()->GetDevice()->CreatePixelShader(
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mPixelShader);
}

void PixelShader::Initialize(const void* buffer, uint32_t size, const char* entryPoint, const char* pixelShaderModel)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompile(buffer, size, nullptr, nullptr, nullptr, entryPoint, pixelShaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr) && errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	SafeRelease(errorBlob);

	// Create pixel buffer
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	hr = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mPixelShader);
	ASSERT(SUCCEEDED(hr), "[PixelShader] Failed to create pixel shader.");

	SafeRelease(shaderBlob);
}

void S::Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void S::Graphics::PixelShader::Bind()
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}