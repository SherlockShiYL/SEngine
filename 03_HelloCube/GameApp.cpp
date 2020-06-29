#include "GameApp.h"
#include <Input/Inc/Input.h>

using namespace S;

//nameless namespace & static	//Internal Linkage
// static cannot be static class()
namespace // Global stuff only used in this cpp file
{
	float gradient = 1280.0f / 720.0f;
	// Define our triangle vertices
	S::Graphics::VertexPC sVertices[8] =
	{
		{{0.5f,0.5f*gradient,0.5f},{1.0f,0.0f,0.0f,1.0f}},		//x y z		0	+++
		{{0.5f,0.5f*gradient,-0.5f},{0.0f,1.0f,0.0f,1.0f}},		//x y -z	1	++-
		{{0.5f,-0.5f*gradient,0.5f},{0.0f,0.0f,1.0f,1.0f}},		//x -y z	2	+-+
		{{0.5f,-0.5f*gradient,-0.5f},{1.0f,0.0f,1.0f,1.0f}},	//x -y -z	3	+--
		{{-0.5f,0.5f*gradient,0.5f},{1.0f,1.0f,0.0f,1.0f}},		//-x y z	4	-++
		{{-0.5f,0.5f*gradient,-0.5f},{0.0f,1.0f,1.0f,1.0f}},	//-x y -z	5	-+-
		{{-0.5f,-0.5f*gradient,0.5f},{0.5f,0.5f,0.5f,1.0f}},	//-x -y z	6	--+
		{{-0.5f,-0.5f*gradient,-0.5f},{1.0f,0.0f,0.0f,1.0f}}	//-x -y -z	7	---
	};
	uint32_t sIndices[36] =
	{
		5,1,3,
		5,3,7,
		0,2,1,
		1,2,3,
		0,1,4,
		4,1,5,
		4,5,7,
		7,6,4,
		0,4,6,
		6,2,0,
		3,6,7,
		3,2,6
	};

	float rotationX = 0.0f;
	float rotationY = 0.0f;
	float rotationZ = 0.0f;

}

GameApp::GameApp()
{}

GameApp::~GameApp()
{}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), GetAppName(), 1280, 720);
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mVertexShader.Initialize(L"../Assets/Shaders/DoTransform.fx", Graphics::VertexPC::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/DoTransform.fx");
	mMeshBuffer.Initialize(sizeof(S::Graphics::VertexPC), sVertices, std::size(sVertices), sIndices, std::size(sIndices));
	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(Math::Matrix4) * 3));
}

void GameApp::OnTerminate()
{
	mConstantBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();

	Input::InputSystem::StaticTerminate();

	Graphics::GraphicsSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	Input::InputSystem::Get()->Update();
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::LEFT))
	{
		rotationY -= 0.02f;
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::RIGHT))
	{
		rotationY += 0.02f;
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::UP))
	{
		rotationX -= 0.02f;
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::DOWN))
	{
		rotationX += 0.02f;
	}

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

	Math::Matrix4 world, view, projection;
	world = Math::Matrix4::RotationY(rotationY)*Math::Matrix4::RotationX(rotationX);
	view = Math::Matrix4
	(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,1.0f,
		0.0f,0.0f,3.0f,1.0f		// Set camera at(0,0,-3), looking at the origin
	);
	projection = Math::Matrix4
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,1.0f,
		0.0f,0.0f,-1.0f,0.0f
	};

	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	// Update matrices
	Math::Matrix4 matrices[3];
	matrices[0] = Math::Transpose(world);
	matrices[1] = Math::Transpose(view);
	matrices[2] = Math::Transpose(projection);
	mConstantBuffer.Set(matrices);
	mConstantBuffer.BindVS();

	mMeshBuffer.Render();
	Graphics::GraphicsSystem::Get()->EndRender();
}