#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	struct TankCData
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
	mWindow.Initialize(GetInstance(), GetAppName(), 1280, 720); //1280
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Graphics::SimpleDraw::Initialize(1000000);
	Graphics::Gui::Initialize(mWindow.GetWindowHandle());

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Clamp);

	mCamera.SetPosition({ 20.0f,0.0f,0.0f });
	mCamera.SetLookAt({ 20.0f,0.0f,50.0f });

	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/skybox01.png", 1000.0f);

	// Tank
	mModelLoader.LoadOBJ("../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(sizeof(S::Graphics::Vertex), mTankMesh.GetVertices(), mTankMesh.GetVertexCount(), mTankMesh.GetIndices(), mTankMesh.GetIndexCount());
	mTankTexture[0].Initialize("../Assets/Models/Tank/tank_ao.jpg");
	mTankTexture[1].Initialize("../Assets/Models/Tank/tank_diffuse.jpg");
	mTankTexture[2].Initialize("../Assets/Models/Tank/tank_specular.jpg");
	mTankVertexShader.Initialize(L"../Assets/Shaders/Tank.fx", Graphics::Vertex::Format);
	mTankPixelShader.Initialize(L"../Assets/Shaders/Tank.fx");
	mTankConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(TankCData)));
}

void GameApp::OnTerminate()
{
	mTankConstantBuffer.Terminate();

	mTankPixelShader.Terminate();
	mTankVertexShader.Terminate();
	for (int i = 0; i < 3; ++i)
	{
		mTankTexture[i].Terminate();
	}
	mTankMeshBuffer.Terminate();
	mTankMesh.Destroy();


	mSkyBox.Terminate();

	mSampler.Terminate();

	Graphics::SimpleDraw::Terminate();
	Graphics::Gui::Terminate();
	Graphics::SimpleDraw::Terminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	// deltaTime
	static auto prevTime = std::chrono::high_resolution_clock::now();
	auto currTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count() / 1000.0f;
	prevTime = currTime;

	{
		auto is = Input::InputSystem::Get();
		is->Update();
		{
			// Move Camera
			if (is->IsKeyDown(Input::KeyCode::W))
			{
				mCamera.Walk(0.1f);
			}
			if (is->IsKeyDown(Input::KeyCode::S))
			{
				mCamera.Walk(-0.1f);
			}
			if (is->IsKeyDown(Input::KeyCode::A))
			{
				mCamera.Strafe(-0.1f);
			}
			if (is->IsKeyDown(Input::KeyCode::D))
			{
				mCamera.Strafe(0.1f);
			}
			if (is->IsKeyDown(Input::KeyCode::SPACE))
			{
				mCamera.Rise(0.1f);
			}
			if (is->IsKeyDown(Input::KeyCode::C))
			{
				mCamera.Rise(-0.1f);
			}
			if (is->IsMouseDown(Input::MouseButton::RBUTTON))
			{
				mCamera.Yaw(is->GetMouseMoveX()*0.002f);
				mCamera.Pitch(is->GetMouseMoveY()*0.002f);
			}
			if (is->IsKeyDown(Input::KeyCode::F))
			{
				mCamera.SetLookAt({ 0.0f,0.0f,0.0f });
			}
			if (is->IsKeyDown(Input::KeyCode::R))
			{
				mCamera.SetPosition(mTankPosition);
			}
			float scale = 10.0f;
			if (is->IsKeyDown(Input::KeyCode::U))
			{
				Math::Quaternion quat = Math::QuaternionRotationAxis(Math::Vector3::XAxis(), deltaTime*scale);
				mQuaternion = mQuaternion * quat;
			}
			if (is->IsKeyDown(Input::KeyCode::I))
			{
				Math::Quaternion quat = Math::QuaternionRotationAxis(Math::Vector3::YAxis(), deltaTime*scale);
				mQuaternion = mQuaternion * quat;
			}
			if (is->IsKeyDown(Input::KeyCode::O))
			{
				Math::Quaternion quat = Math::QuaternionRotationAxis(Math::Vector3::ZAxis(), deltaTime*scale);
				mQuaternion = mQuaternion * quat;
			}
			mRotateMatrix = Math::MatrixRotationQuaternion(mQuaternion);
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

	// Render
	Graphics::GraphicsSystem::Get()->BeginRender();

	mSkyBox.Render(mCamera);

	// Tank
	Math::Matrix4 world, view, projection, lightview, lightprojection;
	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();
	TankCData tankCD;
	world = mRotateMatrix*Math::Translation(mTankPosition);
	tankCD.w = Math::Transpose(world);
	tankCD.v = Math::Transpose(view);
	tankCD.p = Math::Transpose(projection);
	tankCD.l = mDirectionalLight;
	tankCD.m = mMaterial;
	tankCD.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };
	mTankVertexShader.Bind();
	mTankPixelShader.Bind();
	mTankConstantBuffer.Set(&tankCD);
	mTankConstantBuffer.BindVS();
	mTankConstantBuffer.BindPS();

	for (int i = 0; i < 3; ++i)
	{
		mTankTexture[i].BindPS(i);
	}
	mSampler.BindPS();
	mTankMeshBuffer.Render();
	
	Graphics::SimpleDraw::Render(mCamera);

	{
		Graphics::Gui::BeginRender();
		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("FPS: %f", 1.0f / deltaTime);
		/*ImGui::SliderFloat("FOV", &fov, 89.0f, 120.0f);

		ImGui::SliderFloat("nx", &v.x, -1.0f, 1.0f);
		ImGui::SliderFloat("ny", &v.y, -1.0f, 1.0f);
		ImGui::SliderFloat("nz", &v.z, -1.0f, 1.0f);
		vt = Math::Normalize(v);
		ImGui::Text("Normalize:%f %f %f", vt.x, vt.y, vt.z);
		ImGui::Text("Rate:%f %f", v.x/vt.x, v.y/vt.y);

		v2 = Math::Normalize({ vt.x,vt.y });
		ImGui::Text("%f %f", v2.x, v2.y);

		ImGui::SliderFloat(" ", &perc, 0.5f, 1.2f);
		for (int i = 0; i < 6; ++i)
		{
			ImGui::Image(mCubeRenderTarget[i].GetShaderResourceView(), { 150.0f,150.0f });
		}*/
		Graphics::Gui::EndRender();
	}
	Graphics::GraphicsSystem::Get()->EndRender();

}