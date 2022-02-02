#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	float ds = 0.075f;
	struct TerrainCData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
		Math::Matrix4 pv;
		Math::Matrix4 pp;
		Math::Matrix4 sv;
		Math::Matrix4 sp;
		Graphics::DirectionalLight l;
		Graphics::Material m;
		Math::Vector4 vp;
	};
	struct CData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
		Graphics::DirectionalLight l;
		Graphics::Material m;
		Math::Vector4 vp;
	};
	struct DepthCData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
	};
}

GameApp::GameApp()
{}

GameApp::~GameApp()
{}

void GameApp::DepthDraw()
{
	mDepthRenderTarget.BeginRender();

	mDepthVertexShader.Bind();
	mDepthPixelShader.Bind();

	Math::Matrix4 world, view, projection;
	world = Math::Translation(mTankPosition);
	view = mLightCamera.GetViewMatrix();
	projection = mLightCamera.GetPerspectiveMatrix(1.0f);

	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	DepthCData dcd;
	dcd.w = Math::Transpose(world);
	dcd.v = Math::Transpose(view);
	dcd.p = Math::Transpose(projection);

	mDepthConstantBuffer.Set(&dcd);
	mDepthConstantBuffer.BindVS();
	mDepthConstantBuffer.BindPS();

	mTankMeshBuffer.Render();

	mDepthRenderTarget.EndRender();
}

void GameApp::OnInitialize()
{
	mWindow.Initialize(GetInstance(), "ShadowMapping", 1280, 720);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Graphics::SimpleDraw::Initialize(1000000);
	Graphics::Gui::Initialize(mWindow.GetWindowHandle());

	Graphics::MeshBuilder::GenerateTerrain(mMesh,100,20.0f);
	mMeshBuffer.Initialize(sizeof(S::Graphics::Vertex), mMesh.GetVertices(), mMesh.GetVertexCount(), mMesh.GetIndices(), mMesh.GetIndexCount());

	mTexture.Initialize("../Assets/Images/Sand.jpg");
	mSelectionTexture.Initialize("../Assets/Images/BlueSelection.png");
	mSampler.Initialize(S::Graphics::Sampler::Filter::Anisotropic, S::Graphics::Sampler::AddressMode::Clamp);

	mShadowVertexShader.Initialize(L"../Assets/Shaders/ProjectorWithShadow.fx", Graphics::Vertex::Format);
	mShadowPixelShader.Initialize(L"../Assets/Shaders/ProjectorWithShadow.fx");

	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(TerrainCData)));
	mTankConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mModelLoader.LoadOBJ("../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(sizeof(S::Graphics::Vertex), mTankMesh.GetVertices(), mTankMesh.GetVertexCount(), mTankMesh.GetIndices(), mTankMesh.GetIndexCount());
	mTankTexture[0].Initialize("../Assets/Models/Tank/tank_ao.jpg");
	mTankTexture[1].Initialize("../Assets/Models/Tank/tank_diffuse.jpg");
	mTankTexture[2].Initialize("../Assets/Models/Tank/tank_specular.jpg");
	mTankVertexShader.Initialize(L"../Assets/Shaders/Tank.fx", Graphics::Vertex::Format);
	mTankPixelShader.Initialize(L"../Assets/Shaders/Tank.fx");

	mCamera.SetPosition({ 100.0f,20.0f,0.0f });
	mCamera.SetLookAt({ 100.0f,0.0f,50.0f });

	mLightCamera.SetPosition(mTankPosition + Math::Vector3{-20.0f, 20.0f, -20.0f});
	mLightCamera.SetLookAt(mTankPosition);
	mDirectionalLight.mDirection = Math::Normalize({ 20.0f, -20.0f, 20.0f });
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mDepthVertexShader.Initialize(L"../Assets/Shaders/DepthMap.fx", Graphics::Vertex::Format);
	mDepthPixelShader.Initialize(L"../Assets/Shaders/DepthMap.fx");
	mDepthRenderTarget.Initialize(
		720u, 720u,
		Graphics::RenderTarget::Format::RGBA_U8);
	mDepthConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(DepthCData)));
}

void GameApp::OnTerminate()
{
	mDepthConstantBuffer.Terminate();
	mDepthRenderTarget.Terminate();
	mDepthPixelShader.Terminate();
	mDepthVertexShader.Terminate();

	mTankConstantBuffer.Terminate();
	mConstantBuffer.Terminate();

	mTankPixelShader.Terminate();
	mTankVertexShader.Terminate();
	for (int i = 0; i < 3; ++i)
	{
		mTankTexture[i].Terminate();
	}
	mTankMeshBuffer.Terminate();
	mTankMesh.Destroy();

	mShadowVertexShader.Terminate();
	mShadowPixelShader.Terminate();

	mSampler.Terminate();
	mSelectionTexture.Terminate();
	mTexture.Terminate();

	mMeshBuffer.Terminate();

	Graphics::SimpleDraw::Terminate();
	mMesh.Destroy();
	Graphics::Gui::Terminate();
	Graphics::SimpleDraw::Terminate();
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
				mCamera.Walk(0.5f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::S))
			{
				mCamera.Walk(-0.5f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::A))
			{
				mCamera.Strafe(-0.5f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::D))
			{
				mCamera.Strafe(0.5f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::SPACE))
			{
				mCamera.Rise(0.5f);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::C))
			{
				mCamera.Rise(-0.5f);
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
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::R))
	{
		mCamera.SetPosition(mLightCamera.GetPosition());
		mCamera.SetDirection(mLightCamera.GetDirection());
	}

	// deltaTime
	static auto prevTime = std::chrono::high_resolution_clock::now();
	auto currTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count() / 1000.0f;
	prevTime = currTime;

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 world, view, projection, lightview, lightprojection;

	// Terrain
	mShadowVertexShader.Bind();
	mShadowPixelShader.Bind();

	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();
	lightview = mLightCamera.GetViewMatrix();
	lightprojection = mLightCamera.GetPerspectiveMatrix(1.0f);

	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	TerrainCData cd;
	cd.w = Math::Transpose(world);
	cd.v = Math::Transpose(view);
	cd.p = Math::Transpose(projection);
	cd.sv = Math::Transpose(lightview);
	cd.sp = Math::Transpose(lightprojection);
	cd.l = mDirectionalLight;
	cd.m = mMaterial;
	cd.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,ds };

	mConstantBuffer.Set(&cd);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mTexture.BindPS();
	mDepthRenderTarget.BindPS(3);
	mSampler.BindPS();

	mMeshBuffer.Render();
	mDepthRenderTarget.UnbindPS(3);
	// Tank
	CData tankCD;
	tankCD.v = Math::Transpose(view);
	tankCD.p = Math::Transpose(projection);
	tankCD.l = mDirectionalLight;
	tankCD.m = mMaterial;
	tankCD.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,ds };
	mTankVertexShader.Bind();
	mTankPixelShader.Bind();
	world = Math::Translation(mTankPosition);
	tankCD.w = Math::Transpose(world);
	mTankConstantBuffer.Set(&tankCD);
	mTankConstantBuffer.BindVS();
	mTankConstantBuffer.BindPS();

	for (int i = 0; i < 3; ++i)
	{
		mTankTexture[i].BindPS(i);
	}
	mSampler.BindPS();
	mTankMeshBuffer.Render();

	DepthDraw();

	/*for (uint32_t i = 0; i < mMesh.GetIndexCount(); i+=6)
	{
		const auto& a = mMesh.GetVertex(mMesh.GetIndex(i));
		const auto& b = mMesh.GetVertex(mMesh.GetIndex(i + 1));
		const auto& c = mMesh.GetVertex(mMesh.GetIndex(i + 2));
		Graphics::SimpleDraw::AddLine(a.position, b.position, Math::Vector4::Gray());
		Graphics::SimpleDraw::AddLine(a.position, c.position, Math::Vector4::Gray());
	}

	Graphics::SimpleDraw::AddLine({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f }, S::Math::Vector4::Red());
	Graphics::SimpleDraw::AddLine({ 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f }, S::Math::Vector4::Green());*/

	S::Math::Vector3 a{ 80.0f,30.0f,50.0f };
	S::Math::Vector3 b{ 100.0f,70.0f,50.0f };
	S::Math::Vector3 c{ 120.0f,30.0f,50.0f };
	S::Graphics::SimpleDraw::AddLine(a, b, Math::Vector4::Red());
	S::Graphics::SimpleDraw::AddLine(b, c, Math::Vector4::Green());
	S::Graphics::SimpleDraw::AddLine(c, a, Math::Vector4::Blue());
	
	auto ray = mCamera.ScreenPointToRay(S::Input::InputSystem::Get()->GetMouseScreenX(), S::Input::InputSystem::Get()->GetMouseScreenY());

	float distance{ 0.0f };
	if (Geometry::Intersect(ray, a, b, c, distance))
	{
		auto intersectionPoint = ray.org + ray.dir*distance;
		S::Graphics::SimpleDraw::AddSphere(intersectionPoint, 5.0f, Math::Vector4::Yellow());
	}

	Graphics::SimpleDraw::Render(mCamera);

	{
		Graphics::Gui::BeginRender();

		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("FPS: %f", 1.0f / deltaTime);
		ImGui::Image(mDepthRenderTarget.GetShaderResourceView(), { 200.0f,200.0f });

		Graphics::Gui::EndRender();
	}
	Graphics::GraphicsSystem::Get()->EndRender();

}