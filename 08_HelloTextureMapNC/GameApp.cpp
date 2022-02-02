#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

//nameless namespace & static	//Internal Linkage
// static cannot be static class()
namespace // Global stuff only used in this cpp file
{
	float rotationY = 0.0f;
	float la = 1.0f;
	float ld = 1.0f;
	float ls = 1.0f;

	float ma = 1.0f;
	float md = 1.0f;
	float ms = 1.0f;
	float ds = 0.075f;

	bool bN = false;
	bool bT = false;
	bool bB = false;

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

	Graphics::SimpleDraw::Initialize(100000);

	Graphics::MeshBuilder::GenerateSphere(mMesh);
	mMeshBuffer.Initialize(sizeof(S::Graphics::Vertex), mMesh.GetVertices(), mMesh.GetVertexCount(), mMesh.GetIndices(), mMesh.GetIndexCount());

	mTexture[0].Initialize("../Assets/Images/8k_earth_daymap.jpg");
	mTexture[1].Initialize("../Assets/Images/8k_earth_nightmap.jpg");
	mTexture[2].Initialize("../Assets/Images/8k_earth_specularmap.jpg");
	mTexture[3].Initialize("../Assets/Images/10k_earth_bumpmap.jpg");
	mTexture[4].Initialize("../Assets/Images/earth_normal.jpg");

	mSampler.Initialize(S::Graphics::Sampler::Filter::Anisotropic, S::Graphics::Sampler::AddressMode::Clamp);
	
	mVertexShader.Initialize(L"../Assets/Shaders/TextureMapping_NightCloud.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/TextureMapping_NightCloud.fx");

	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
}

void GameApp::OnTerminate()
{
	mConstantBuffer.Terminate();
	for (int i = 0; i < 5; ++i)
	{
		mTexture[i].Terminate();
	}

	mSampler.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mMeshBuffer.Terminate();

	Graphics::SimpleDraw::Terminate();

	mMesh.Destroy();
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
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::C))
			{
				mCamera.Rise(-0.05f);
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

	rotationY -= 0.001f;

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 world, view, projection;
	world = Math::Matrix4::RotationY(rotationY);

	// All rendering happens here, between BeginRender and EndRender

	mVertexShader.Bind();
	mPixelShader.Bind();

	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();

	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	CData cd;
	cd.w = Math::Transpose(world);
	cd.v = Math::Transpose(view);
	cd.p = Math::Transpose(projection);
	cd.l = mDirectionalLight;
	cd.m = mMaterial;
	cd.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,ds };

	mConstantBuffer.Set(&cd);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mTexture[0].BindPS();
	mTexture[1].BindPS(1);
	mTexture[2].BindPS(2);
	mTexture[3].BindVS(3);
	mTexture[4].BindPS(4);

	mSampler.BindPS();

	mMeshBuffer.Render();

	if (bN)
	{
		auto SV = mMesh.GetVertices();
		for (uint32_t i = 0; i < mMesh.GetVertexCount(); i++)
		{
			auto wp = Math::Normalize(Math::TransformCoord(SV[i].position, world));
			auto wt = Math::TransformCoord(SV[i].normal, world);
			Graphics::SimpleDraw::AddLine(wp, wp + wt, { wt.x,wt.y,wt.z,1.0f });
		}
	}
	if (bT)
	{
		auto SV = mMesh.GetVertices();
		for (uint32_t i = 0; i < mMesh.GetVertexCount(); i++)
		{
			auto wp = Math::Normalize(Math::TransformCoord(SV[i].position, world));
			auto wt = Math::TransformCoord(SV[i].tangent, world);
			Graphics::SimpleDraw::AddLine(wp, wp + wt, { wt.x,wt.y,wt.z,1.0f });
		}
	}
	if (bB)
	{
		auto SV = mMesh.GetVertices();
		for (uint32_t i = 0; i < mMesh.GetVertexCount(); i++)
		{
			auto wp = Math::Normalize(Math::TransformCoord(SV[i].position, world));
			auto wt = Math::TransformCoord(Math::Cross(SV[i].normal, SV[i].tangent), world);
			Graphics::SimpleDraw::AddLine(wp, wp + wt, { wt.x,wt.y,wt.z,1.0f });
		}
	}

	//Geometry::Sphere tsphere({ mPointLight.mPosition.x,mPointLight.mPosition.y,mPointLight.mPosition.z }, 0.1f);
	//Graphics::SimpleDraw::AddSphere(tsphere, S::Math::Vector4::White(), 16, 16);
	Graphics::SimpleDraw::Render(mCamera);
	auto sv = mMesh.GetVertices();
	{
		Graphics::Gui::BeginRender();
		//ImGui::ShowDemoWindow();

		ImGui::SliderFloat("LAmbient", &la, 0.0f, 1.0f);
		ImGui::SliderFloat("LDiffuse", &ld, 0.0f, 1.0f);
		ImGui::SliderFloat("LSpecular", &ls, 0.0f, 1.0f);

		ImGui::SliderFloat("MAmbient", &ma, 0.0f, 1.0f);
		ImGui::SliderFloat("MDiffuse", &md, 0.0f, 1.0f);
		ImGui::SliderFloat("MSpecular", &ms, 0.0f, 1.0f);
		ImGui::SliderFloat("MPower", &mMaterial.mPower, 0.0f, 50.0f);
		ImGui::SliderFloat("DisplacementScale", &ds, 0.0f, 1.0f);

		ImGui::Checkbox("DrawNormal", &bN);
		ImGui::Checkbox("DrawTangent", &bT);
		ImGui::Checkbox("DrawBinormal", &bB);

		{
			mDirectionalLight.mAmbient.SetEachElement(la);
			mDirectionalLight.mDiffuse.SetEachElement(ld);
			mDirectionalLight.mSpecular.SetEachElement(ls);
			mMaterial.mAmbient.SetEachElement(ma);
			mMaterial.mDiffuse.SetEachElement(md);
			mMaterial.mSpecular.SetEachElement(ms);
		}
		Graphics::Gui::EndRender();
	}
	Graphics::GraphicsSystem::Get()->EndRender();
}