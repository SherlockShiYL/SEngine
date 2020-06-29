#include "GameApp.h"
#include <Input/Inc/Input.h>

using namespace S;

//nameless namespace & static	//Internal Linkage
// static cannot be static class()
namespace // Global stuff only used in this cpp file
{
	float rotationY = 0.0f;
	float planetSize[3]{ 10.0f,3.0f,1.0f };
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

	Graphics::SimpleDraw::Initialize(100000);

	Graphics::MeshBuilder::GenerateSpherePT(mMeshPT[0], 32, 32);
	mMeshBuffer[0].Initialize(sizeof(S::Graphics::VertexPT), mMeshPT[0].GetVertices(), mMeshPT[0].GetVertexCount(), mMeshPT[0].GetIndices(), mMeshPT[0].GetIndexCount());
	Graphics::MeshBuilder::GenerateSkySphere(mMeshPT[3]);
	mMeshBuffer[1].Initialize(sizeof(S::Graphics::VertexPT), mMeshPT[3].GetVertices(), mMeshPT[3].GetVertexCount(), mMeshPT[3].GetIndices(), mMeshPT[3].GetIndexCount());

	mTexture[0].Initialize("../Assets/Images/8k_sun.jpg");
	mTexture[1].Initialize("../Assets/Images/8k_earth_daymap.jpg");
	mTexture[2].Initialize("../Assets/Images/2k_moon.jpg");
	mTexture[3].Initialize("../Assets/Images/8k_stars_milky_way.jpg");
	mSampler.Initialize(S::Graphics::Sampler::Filter::Anisotropic, S::Graphics::Sampler::AddressMode::Clamp);
	
	mVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::VertexPT::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(Math::Matrix4) * 3));

	mCamera.SetPosition({ 20.0f,50.0f,40.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,40.0f });
}

void GameApp::OnTerminate()
{
	mSampler.Terminate();
	for (int i = 0; i < 4; ++i)
	{
		mTexture[i].Terminate();
	}
	mConstantBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer[0].Terminate();
	mMeshBuffer[1].Terminate();

	Graphics::SimpleDraw::Terminate();

	Input::InputSystem::StaticTerminate();

	Graphics::GraphicsSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	{
		Input::InputSystem::Get()->Update();
		{
			// Move Camera
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::W))
			{
				mCamera.Walk(0.2f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::S))
			{
				mCamera.Walk(-0.2f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::A))
			{
				mCamera.Strafe(-0.2f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::D))
			{
				mCamera.Strafe(0.2f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::SPACE))
			{
				mCamera.Rise(0.2f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::LSHIFT))
			{
				mCamera.Rise(-0.2f);
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
		}
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

	rotationY -= 0.01f;

	Graphics::GraphicsSystem::Get()->BeginRender();

	// All rendering happens here, between BeginRender and EndRender
	mVertexShader.Bind();
	mPixelShader.Bind();

	Math::Matrix4 world[4], view, projection;

	world[0] = Math::Translation({ 0.0f,0.0f,0.0f })*Math::Matrix4::Scaling(planetSize[0]);
	world[1] = Math::Matrix4::RotationY(rotationY*3.0f)*Math::Matrix4::Scaling(planetSize[1])*Math::Translation({ 0.0f,0.0f,40.0f })*Math::Matrix4::RotationY(rotationY*0.1f);
	world[2] = Math::Matrix4::RotationY(rotationY*0.1f)*Math::Translation({ 0.0f,0.0f,10.0f })*Math::Matrix4::RotationY(rotationY)*Math::Translation({ 0.0f,0.0f,40.0f })*Math::Matrix4::RotationY(rotationY*0.1f);

	for (int i = 0; i < 3; ++i)
	{
		view = mCamera.GetViewMatrix();
		projection = mCamera.GetPerspectiveMatrix();

		ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

		// Update matrices
		Math::Matrix4 matrices[3];
		matrices[0] = Math::Transpose(world[i]);
		matrices[1] = Math::Transpose(view);
		matrices[2] = Math::Transpose(projection);

		mConstantBuffer.Set(matrices);
		mConstantBuffer.BindVS();

		mTexture[i].BindPS();
		mSampler.BindPS();

		mMeshBuffer[0].Render();

		Geometry::Sphere tsphere1({ world[i]._41,world[i]._42,world[i]._43 }, planetSize[i] + 0.2f);
		//Graphics::SimpleDraw::AddSphere(tsphere1, S::Math::Vector4::Cyan(), 16, 16);
	}

	world[3] = Math::Translation(mCamera.GetPosition());

	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();

	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	// Update matrices
	Math::Matrix4 matrices[3];
	matrices[0] = Math::Transpose(world[3]);
	matrices[1] = Math::Transpose(view);
	matrices[2] = Math::Transpose(projection);

	mConstantBuffer.Set(matrices);
	mConstantBuffer.BindVS();

	mTexture[3].BindPS();
	mSampler.BindPS();

	mMeshBuffer[1].Render();

	Graphics::SimpleDraw::Render(mCamera);

	Graphics::GraphicsSystem::Get()->EndRender();
}