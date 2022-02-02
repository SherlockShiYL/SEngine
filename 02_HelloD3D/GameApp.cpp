#include "GameApp.h"

using namespace S;

//nameless namespace & static	//Internal Linkage
// static cannot be static class()
namespace // Global stuff only used in this cpp file
{
	// Define our triangle vertices
	S::Graphics::VertexPC sVertices[37] = 
	{
		{{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}}
	};
	uint32_t sIndices[108] =
	{};
}

GameApp::GameApp()
{}

GameApp::~GameApp()
{}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), "Hello D3D", 1280, 720);
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);

	float WHRate = 1280.0f / 720.0f;
	float gradient  = 1.0f / 6.0f;
	for (int i = 0; i < 36; ++i)
	{
		sVertices[i + 1].position = { 0.5f*cos((90.0f - static_cast<float>(i)*10.0f)*S::Math::kDegToRad),0.5f*sin((90.0f - static_cast<float>(i)*10.0f)*S::Math::kDegToRad)*WHRate,0.0f };
	}
	for (int i = 0; i < 6; ++i)
	{
		sVertices[i + 1].color = { 1.0f,0.0f + (static_cast<float>(i) * gradient),0.0f,1.0f };
	}
	for (int i = 6; i < 12; ++i)
	{
		sVertices[i + 1].color = { 1.0f - (static_cast<float>(i - 6) * gradient),1.0f,0.0f,1.0f };
	}
	for (int i = 12; i < 18; ++i)
	{
		sVertices[i + 1].color = { 0.0f,1.0f,0.0f + (static_cast<float>(i - 12) * gradient),1.0f };
	}
	for (int i = 18; i < 24; ++i)
	{
		sVertices[i + 1].color = { 0.0f,1.0f - (static_cast<float>(i - 18) * gradient),1.0f ,1.0f };
	}
	for (int i = 24; i < 30; ++i)
	{
		sVertices[i + 1].color = { 0.0f + (static_cast<float>(i - 24) * gradient),0.0f,1.0f ,1.0f };
	}
	for (int i = 30; i < 36; ++i)
	{
		sVertices[i + 1].color = { 1.0f,0.0f,1.0f - (static_cast<float>(i - 30) * gradient),1.0f };
	}

	for (int i = 0; i < 107; ++i)
	{
		if (i % 3 == 0)
		{
			sIndices[i] = 0;
		}
		else if (i % 3 == 1)
		{
			sIndices[i] = i / 3 + 1;
		}
		else
		{
			sIndices[i] = sIndices[i - 1] + 1;
		}
	}
	sIndices[107] = 1;
	mVertexShader.Initialize(L"../Assets/Shaders/DoSomething.fx", Graphics::VertexPC::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/DoSomething.fx");
	mMeshBuffer.Initialize(sizeof(S::Graphics::VertexPC), sVertices, std::size(sVertices), sIndices, std::size(sIndices));
}

void GameApp::OnTerminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();

	Graphics::GraphicsSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate(bool(*GameLoop)(float))
{
	if (mWindow.ProcessMessage())
	{
		Kill();
		return;
	}

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
		return;
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	// All rendering happens here, between BeginRender and EndRender
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();

	Graphics::GraphicsSystem::Get()->EndRender();
}