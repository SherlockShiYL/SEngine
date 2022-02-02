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
	int nums = 3;
	Math::Vector4 distance{ 0.00f,0.0f,0.0f,0.0f };
	float coord = 0.001f;

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
	mMeshBuffer.Initialize(sizeof(Graphics::Vertex), mMesh.GetVertices(), mMesh.GetVertexCount(), mMesh.GetIndices(), mMesh.GetIndexCount());

	mTexture[0].Initialize("../Assets/Images/8k_earth_daymap.jpg");
	mTexture[1].Initialize("../Assets/Images/8k_earth_specularmap.jpg");
	mTexture[2].Initialize("../Assets/Images/10k_earth_bumpmap.jpg");
	mTexture[3].Initialize("../Assets/Images/earth_normal.jpg");

	mSampler[0].Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Clamp);
	
	mVertexShader.Initialize(L"../Assets/Shaders/TextureMapping.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/TextureMapping.fx");

	mPostProcessVertexShader.Initialize(L"../Assets/Shaders/PostProcessing.fx", Graphics::VertexPT::Format);
	mPostProcessPixelShader.Initialize(L"../Assets/Shaders/PostProcessing.fx");
	for (int i = 0; i < 3; ++i)
	{
		mRenderTarget[i].Initialize(
			Graphics::GraphicsSystem::Get()->GetWidth(),
			Graphics::GraphicsSystem::Get()->GetHeight(),
			Graphics::RenderTarget::Format::RGBA_U8);
	}

	mSampler[1].Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp);

	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));
	mConstantBufferDis.Initialize(sizeof(Math::Vector4));

	constexpr Graphics::VertexPT screenVertices[]=
	{
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},
		{{-1.0f,1.0f,0.0f},{0.0f,0.0f}},
		{{1.0f,1.0f,0.0f},{1.0f,0.0f}},
		{{1.0f,-1.0f,0.0f},{1.0f,1.0f}}
	};
	constexpr uint32_t screenIndices[] =
	{
		0,1,2,0,2,3
	};

	mScreenMeshBuffer.Initialize(sizeof(Graphics::VertexPT), screenVertices, std::size(screenVertices), screenIndices, std::size(screenIndices));
	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
}

void GameApp::OnTerminate()
{
	mConstantBufferDis.Terminate();
	mConstantBuffer.Terminate();
	mScreenMeshBuffer.Terminate();

	for (int i = 0; i < 3; ++i)
	{
		mRenderTarget[i].Terminate();
	}
	mPostProcessVertexShader.Terminate();
	mPostProcessPixelShader.Terminate();

	mVertexShader.Terminate();
	mPixelShader.Terminate();

	mMeshBuffer.Terminate();
	for (int i = 0; i < 4; ++i)
	{
		mTexture[i].Terminate();
	}
	mSampler[1].Terminate();
	mSampler[0].Terminate();

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

	// deltaTime
	static auto prevTime = std::chrono::high_resolution_clock::now();
	auto currTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count() / 1000.0f;
	prevTime = currTime;

	//rotationY -= 0.001f;
	distance.x -= coord;
	Graphics::GraphicsSystem::Get()->BeginRender();

	DrawScene();
	//ProcessScene1();
	ProcessScene2(nums);

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
		ImGui::SliderInt("Num of Loop", &nums, 0, 10);

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

void GameApp::DrawScene()
{
	mRenderTarget[0].BeginRender();

	// All rendering happens here, between BeginRender and EndRender
	mVertexShader.Bind();
	mPixelShader.Bind();

	Math::Matrix4 world, view, projection;
	world = Math::Matrix4::RotationY(rotationY);
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
	mTexture[2].BindVS(2);
	mTexture[3].BindPS(3);

	mSampler[0].BindPS();

	mMeshBuffer.Render();

	mRenderTarget[0].EndRender();
}

void GameApp::ProcessScene1()
{
	mRenderTarget[1].BeginRender();

	mPostProcessPixelShader.Bind();
	mPostProcessVertexShader.Bind();

	distance.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth());
	distance.z = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight());
	distance.w = 0.0f;

	mConstantBufferDis.Set(&distance);
	mConstantBufferDis.BindVS();
	mConstantBufferDis.BindPS();

	mRenderTarget[0].BindPS();
	mSampler[1].BindPS();

	mScreenMeshBuffer.Render();
	mRenderTarget[0].UnbindPS();

	mRenderTarget[1].EndRender();

	mPostProcessPixelShader.Bind();
	mPostProcessVertexShader.Bind();

	distance.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth());
	distance.z = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight());
	distance.w = 1.0f;

	mConstantBufferDis.Set(&distance);
	mConstantBufferDis.BindVS();
	mConstantBufferDis.BindPS();

	mRenderTarget[1].BindPS();
	mSampler[1].BindPS();

	mScreenMeshBuffer.Render();
	mRenderTarget[1].UnbindPS();
}

void GameApp::ProcessScene2(int num)
{
	mRenderTarget[1].BeginRender();

	mPostProcessPixelShader.Bind();
	mPostProcessVertexShader.Bind();

	distance.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth());
	distance.z = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight());
	distance.w = 0.0f;

	mConstantBufferDis.Set(&distance);
	mConstantBufferDis.BindVS();
	mConstantBufferDis.BindPS();

	mRenderTarget[0].BindPS();
	mSampler[1].BindPS();

	mScreenMeshBuffer.Render();
	mRenderTarget[0].UnbindPS();

	mRenderTarget[1].EndRender();

	mRenderTarget[2].BeginRender();

	mPostProcessPixelShader.Bind();
	mPostProcessVertexShader.Bind();

	distance.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth());
	distance.z = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight());
	distance.w = 1.0f;

	mConstantBufferDis.Set(&distance);
	mConstantBufferDis.BindVS();
	mConstantBufferDis.BindPS();

	mRenderTarget[1].BindPS();
	mSampler[1].BindPS();

	mScreenMeshBuffer.Render();
	mRenderTarget[1].UnbindPS();

	mRenderTarget[2].EndRender();

	for (int i = 0; i < num; ++i)
	{
		mRenderTarget[1].BeginRender();

		mPostProcessPixelShader.Bind();
		mPostProcessVertexShader.Bind();

		distance.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth());
		distance.z = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight());
		distance.w = 0.0f;

		mConstantBufferDis.Set(&distance);
		mConstantBufferDis.BindVS();
		mConstantBufferDis.BindPS();

		mRenderTarget[2].BindPS();
		mSampler[1].BindPS();

		mScreenMeshBuffer.Render();
		mRenderTarget[2].UnbindPS();

		mRenderTarget[1].EndRender();

		mRenderTarget[2].BeginRender();

		mPostProcessPixelShader.Bind();
		mPostProcessVertexShader.Bind();

		distance.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth());
		distance.z = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight());
		distance.w = 1.0f;

		mConstantBufferDis.Set(&distance);
		mConstantBufferDis.BindVS();
		mConstantBufferDis.BindPS();

		mRenderTarget[1].BindPS();
		mSampler[1].BindPS();

		mScreenMeshBuffer.Render();
		mRenderTarget[1].UnbindPS();

		mRenderTarget[2].EndRender();
	}

	mPostProcessPixelShader.Bind();
	mPostProcessVertexShader.Bind();

	distance.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth());
	distance.z = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY()) / static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight());
	distance.w = 1.0f;

	mConstantBufferDis.Set(&distance);
	mConstantBufferDis.BindVS();
	mConstantBufferDis.BindPS();

	mRenderTarget[1].BindPS();
	mSampler[1].BindPS();

	mScreenMeshBuffer.Render();
	mRenderTarget[1].UnbindPS();
}
