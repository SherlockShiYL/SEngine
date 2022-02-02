#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

//nameless namespace & static	//Internal Linkage
// static cannot be static class()
namespace // Global stuff only used in this cpp file
{
	float rotationY = 0.0f;
	float la = 0.3f;
	float ld = 0.7f;
	float ls = 0.7f;

	float ma = 0.3f;
	float md = 0.7f;
	float ms = 0.7f;

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
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Graphics::Gui::Initialize(mWindow.GetWindowHandle());

	Graphics::MeshBuilder::GenerateSpherePNC(mMeshPNC,64,33);
	mMeshBuffer[0].Initialize(sizeof(S::Graphics::VertexPNC), mMeshPNC.GetVertices(), mMeshPNC.GetVertexCount(), mMeshPNC.GetIndices(), mMeshPNC.GetIndexCount());
	
	Graphics::MeshBuilder::GenerateSpherePNT(mMeshPNT,64,33);
	mMeshBuffer[1].Initialize(sizeof(S::Graphics::VertexPNT), mMeshPNT.GetVertices(), mMeshPNT.GetVertexCount(), mMeshPNT.GetIndices(), mMeshPNT.GetIndexCount());

	mTexture.Initialize("../Assets/Images/8k_earth_daymap.jpg");

	mSampler.Initialize(S::Graphics::Sampler::Filter::Anisotropic, S::Graphics::Sampler::AddressMode::Clamp);
	
	mVertexShader[0].Initialize(L"../Assets/Shaders/Phong_LightingColor.fx", Graphics::VertexPNC::Format);
	mPixelShader[0].Initialize(L"../Assets/Shaders/Phong_LightingColor.fx");

	mVertexShader[1].Initialize(L"../Assets/Shaders/Phong_ShadingColorDirectional.fx", Graphics::VertexPNC::Format);
	mPixelShader[1].Initialize(L"../Assets/Shaders/Phong_ShadingColorDirectional.fx");

	mVertexShader[2].Initialize(L"../Assets/Shaders/Phong_Shading.fx", Graphics::VertexPNT::Format);
	mPixelShader[2].Initialize(L"../Assets/Shaders/Phong_Shading.fx");

	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mCamera.SetPosition({ 2.5f,1.5f,-5.0f });
}

void GameApp::OnTerminate()
{
	mConstantBuffer.Terminate();
	mSampler.Terminate();

	mTexture.Terminate();

	mVertexShader[0].Terminate();
	mPixelShader[0].Terminate();
	
	mVertexShader[1].Terminate();
	mPixelShader[1].Terminate();
	
	mVertexShader[2].Terminate();
	mPixelShader[2].Terminate();

	mMeshBuffer[0].Terminate();
	mMeshBuffer[1].Terminate();

	Graphics::Gui::Terminate();
	Graphics::GraphicsSystem::StaticTerminate();
	Input::InputSystem::StaticTerminate();
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
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::C))
			{
				mCamera.Rise(-0.2f);
			}
			if (Input::InputSystem::Get()->IsMouseDown(Input::MouseButton::RBUTTON))
			{
				mCamera.Yaw(Input::InputSystem::Get()->GetMouseMoveX()*0.002f);
				mCamera.Pitch(Input::InputSystem::Get()->GetMouseMoveY()*0.002f);
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

	//rotationY -= 0.01f;

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 world[3], view, projection;
	world[0] = Math::Matrix4::Scaling(2.0f)*Math::Matrix4::RotationY(rotationY);
	world[1] = Math::Matrix4::Scaling(2.0f)*Math::Matrix4::RotationY(rotationY)*Math::Translation({ 5.0f,0.0f,0.0f });
	world[2] = Math::Matrix4::Scaling(2.0f)*Math::Matrix4::RotationY(rotationY)*Math::Translation({ 2.5f,4.0f,0.0f });

	// All rendering happens here, between BeginRender and EndRender
	for (int i = 0; i < 3; ++i)
	{
		mVertexShader[i].Bind();
		mPixelShader[i].Bind();

		view = mCamera.GetViewMatrix();
		projection = mCamera.GetPerspectiveMatrix();

		ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

		CData cd;
		cd.w = Math::Transpose(world[i]);
		cd.v = Math::Transpose(view);
		cd.p = Math::Transpose(projection);
		cd.l = mLight;
		cd.m = mMaterial;
		cd.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };

		mConstantBuffer.Set(&cd);

		mConstantBuffer.BindVS();
		mConstantBuffer.BindPS();

		if (i < 2)
		{
			mMeshBuffer[0].Render();
		}
		else
		{
			mTexture.BindPS();
			mSampler.BindPS();
			mMeshBuffer[1].Render();
		}
	}

	Graphics::Gui::BeginRender();
	//ImGui::ShowDemoWindow();

	ImGui::SliderFloat("LAmbient", &la, 0.0f, 1.0f);
	ImGui::SliderFloat("LDiffuse", &ld, 0.0f, 1.0f);
	ImGui::SliderFloat("LSpecular", &ls, 0.0f, 1.0f);

	ImGui::SliderFloat("MAmbient", &ma, 0.0f, 1.0f);
	ImGui::SliderFloat("MDiffuse", &md, 0.0f, 1.0f);
	ImGui::SliderFloat("MSpecular", &ms, 0.0f, 1.0f);

	{
		mLight.mAmbient.SetEachElement(la);
		mLight.mDiffuse.SetEachElement(ld);
		mLight.mSpecular.SetEachElement(ls);
		mMaterial.mAmbient.SetEachElement(ma);
		mMaterial.mDiffuse.SetEachElement(md);
		mMaterial.mSpecular.SetEachElement(ms);
	}
	Graphics::Gui::EndRender();

	Graphics::GraphicsSystem::Get()->EndRender();
}