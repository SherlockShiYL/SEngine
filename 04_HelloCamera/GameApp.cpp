#include "GameApp.h"
#include <Input/Inc/Input.h>

using namespace S;

//nameless namespace & static	//Internal Linkage
// static cannot be static class()
namespace // Global stuff only used in this cpp file
{
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

	Graphics::MeshBuilder::GenerateCubePC(mMesh);

	mVertexShader.Initialize(L"../Assets/Shaders/DoTransform.fx", Graphics::VertexPC::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/DoTransform.fx");
	mMeshBuffer.Initialize(sizeof(S::Graphics::VertexPC), mMesh.GetVertices(), mMesh.GetVertexCount(), mMesh.GetIndices(), mMesh.GetIndexCount());
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

	// Move Camera
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::W))
	{
		mCamera.Walk(0.05f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::S))
	{
		mCamera.Walk(-0.05f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::A))
	{
		mCamera.Strafe(-0.05f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::D))
	{
		mCamera.Strafe(0.05f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::SPACE))
	{
		mCamera.Rise(0.05f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::LSHIFT))
	{
		mCamera.Rise(-0.05f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::J))
	{
		mCamera.Yaw(-0.02f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::L))
	{
		mCamera.Yaw(0.02f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::I))
	{
		mCamera.Pitch(-0.02f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::K))
	{
		mCamera.Pitch(0.02f);
	}
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::F))
	{
		mCamera.SetLookAt({ 0.0f,0.0f,0.0f });
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


	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			for (int k = 0; k < 10; ++k)
			{
				Math::Matrix4 world, view, projection;
				world = Math::Matrix4::RotationY(rotationY)*Math::Matrix4::RotationX(rotationX)*Math::Translation({ i*3.0f,j*3.0f,k*3.0f });

				view = mCamera.GetViewMatrix();
				projection = mCamera.GetPerspectiveMatrix();

				ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

				// Update matrices
				Math::Matrix4 matrices[3];
				matrices[0] = Math::Transpose(world);
				matrices[1] = Math::Transpose(view);
				matrices[2] = Math::Transpose(projection);
				mConstantBuffer.Set(matrices);
				mConstantBuffer.BindVS();

				mMeshBuffer.Render();
			}
		}
	}


	Graphics::GraphicsSystem::Get()->EndRender();
}