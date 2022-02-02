#include "GameApp.h"
#include <Input/Inc/Input.h>

using namespace S;

//nameless namespace & static	//Internal Linkage
// static cannot be static class()
namespace // Global stuff only used in this cpp file
{
	float rotationY = 0.0f;
	struct CData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
		Graphics::DirectionalLight l;
		Graphics::Material m;
		Math::Vector4 vp;
	};
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

	Graphics::MeshBuilder::GenerateSpherePNT(mMeshPNT, 32, 32);
	mMeshBuffer.Initialize(sizeof(S::Graphics::VertexPNT), mMeshPNT.GetVertices(), mMeshPNT.GetVertexCount(), mMeshPNT.GetIndices(), mMeshPNT.GetIndexCount());

	mTexture.Initialize("../Assets/Images/8k_earth_daymap.jpg");
	mSampler.Initialize(S::Graphics::Sampler::Filter::Anisotropic, S::Graphics::Sampler::AddressMode::Clamp);
	
	mVertexShader.Initialize(L"../Assets/Shaders/Phong_Lighting.fx", Graphics::VertexPNT::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Phong_Lighting.fx");

	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mCamera.SetPosition({ 20.0f,50.0f,40.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,40.0f });
}

void GameApp::OnTerminate()
{
	mSampler.Terminate();

	mTexture.Terminate();
	
	mConstantBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();

	Graphics::SimpleDraw::Terminate();

	Input::InputSystem::StaticTerminate();

	Graphics::GraphicsSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate(bool(*GameLoop)(float))
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

	Math::Matrix4 world, view, projection;

	world = Math::Translation({ 0.0f,0.0f,0.0f })*Math::Matrix4::Scaling(10.0f)*Math::Matrix4::RotationY(rotationY);

	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();

	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();
		
	CData cd;
	cd.w = Math::Transpose(world);
	cd.v = Math::Transpose(view);
	cd.p = Math::Transpose(projection);
	cd.l = mLight;
	cd.m = mMaterial;
	cd.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };

	mConstantBuffer.Set(&cd);
		
	mConstantBuffer.BindVS();

	mTexture.BindPS();
	mSampler.BindPS();

	mMeshBuffer.Render();

	Geometry::Sphere tsphere1({ world._41,world._42,world._43 }, 10.2f);
	Graphics::SimpleDraw::AddSphere(tsphere1, S::Math::Vector4::Cyan(), 16, 16);
	

	//Graphics::SimpleDraw::Render(mCamera);

	Graphics::GraphicsSystem::Get()->EndRender();
}