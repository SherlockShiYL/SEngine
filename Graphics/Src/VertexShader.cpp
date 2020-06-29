#include "Precompiled.h"
#include "VertexShader.h"

#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace S;
using namespace S::Graphics;

namespace
{
	// In order for the Input Assembler to extract our vertx data
	// proper, we will define a description for our vertx here
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout(int vertexFormat)
	{
		switch (vertexFormat)
		{
		case VertexP::Format:
		{
			return{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
			};
		}
		case VertexPC::Format:
		{
			return
			{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
		}
		case VertexPNC::Format:
		{
			return
			{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
		}
		case VertexPT::Format:
		{
			return
			{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
		}
		case VertexPNT::Format:
		{
			return
			{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
		}
		case Vertex::Format:
		{
			return
			{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
		}
		case BoneVertex::Format:
		{
			return
			{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"BLENDINDICES",0,DXGI_FORMAT_R32G32B32A32_SINT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"BLENDWEIGHT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
		}
		default:
		{
			ASSERT(false, "Unknow vertex format.");
		}
		}
		return {};
	}
}

void VertexShader::Initialize(const wchar_t* fileName, int vertexFormat)
{
	// Compile the vertex shader code
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		fileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_4_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob != nullptr)
		{
			ASSERT(false, "[VertexShader] Failed to compile shader. Error: %s",
				static_cast<char*>(errorBlob->GetBufferPointer()));
		}
		else
		{
			ASSERT(false, "[VertexShader] Failed to compile shader. HRESULT: %d", hr);
		}
	}
	// Create vertex shader
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateVertexShader(
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mVertexShader);
	
	// Get the vertex layout
	auto vertexLayout = GetVertexLayout(vertexFormat);

	// Create the input layout
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void VertexShader::Initialize(const void* buffer, uint32_t size, uint32_t vertexFormat, const char* entryPoint, const char* shaderModel)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompile(buffer, size, nullptr, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr) && errorBlob != nullptr)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	SafeRelease(errorBlob);

	// Create vertex buffer
	hr = GraphicsSystem::Get()->GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mVertexShader);

	ASSERT(SUCCEEDED(hr), "[VertedShader] Failed to create vertex shader.");

	// Get the vertex layout
	auto vertexLayout = GetVertexLayout(vertexFormat);

	// Create the input layout
	hr = GraphicsSystem::Get()->GetDevice()->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);

	ASSERT(SUCCEEDED(hr), "[VertexShader] Failed to create input layout.");

	SafeRelease(shaderBlob);
}

void VertexShader::Terminate()
{
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);
}

void VertexShader::Bind()
{
	// Get the D3D context and bind our vertex shader, pixel shader, and input layout.
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
}
