#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	uint32_t test = 0;
	float timeLine = 0.0f;
	struct DuckCData
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

	mCamera.SetPosition({ 0.0f,10.0f,-30.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/skybox01.png", 1000.0f);

	// Duck
	mModelLoader.LoadOBJVertex("../Assets/Models/Duck/duck.txt", 0.05f, mDuckMesh);
	mDuckMeshBuffer.Initialize(sizeof(Graphics::Vertex), mDuckMesh.GetVertices(), mDuckMesh.GetVertexCount(), mDuckMesh.GetIndices(), mDuckMesh.GetIndexCount());
	mDuckTexture.Initialize("../Assets/Models/Duck/duck.png");
	mDuckVertexShader.Initialize(L"../Assets/Shaders/SkyBox.fx", Graphics::Vertex::Format);
	mDuckPixelShader.Initialize(L"../Assets/Shaders/SkyBox.fx");
	mDuckConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(DuckCData)));

	Math::Quaternion quat = Math::QuaternionRotationAxis(Math::Vector3::YAxis(), 10.0f);
	mQuaternion = mQuaternion * quat;

	mAnimation = Graphics::AnimationBuilder()
		.AddScaleKey({ 1.0f,1.0f,1.0f }, 5.0f)
		.AddPositionKey({ 0.0f,0.0f,0.0f }, 10.0f)
		.AddPositionKey({ 0.0f,0.0f,0.0f }, 0.0f)
		.AddRotationkey(Math::Quaternion::Identity(), 0.0f)
		.AddPositionKey({ 30.0f,0.0f,20.0f }, 5.0f)
		.AddScaleKey({ 1.0f,1.0f,1.0f }, 0.0f)
		.AddPositionKey({ 30.0f,0.0f,0.0f }, 3.0f)
		.AddRotationkey(mQuaternion, 5.0f)
		.AddRotationkey(Math::Quaternion::Identity(), 7.0f)
		.Finalize();
}

void GameApp::OnTerminate()
{
	mDuckConstantBuffer.Terminate();

	mDuckPixelShader.Terminate();
	mDuckVertexShader.Terminate();
	
	mDuckTexture.Terminate();
	mDuckMeshBuffer.Terminate();
	mDuckMesh.Destroy();

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
				mCamera.SetPosition(mDuckPosition);
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

	// Render
	Graphics::GraphicsSystem::Get()->BeginRender();

	mSkyBox.Render(mCamera);

	/*if (test < mDuckMesh.GetIndexCount())
	{
		test += 3;
	}
	mDuckMeshBuffer.Initialize(sizeof(S::Graphics::Vertex), mDuckMesh.GetVertices(), mDuckMesh.GetVertexCount(), mDuckMesh.GetIndices(), test);*/

	// Duck
	Math::Matrix4 world, view, projection, lightview, lightprojection;
	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();
	DuckCData DuckCD;
	
	
	timeLine += deltaTime;
	world = mAnimation.GetTransform(timeLine);


	DuckCD.w = Math::Transpose(world);
	DuckCD.v = Math::Transpose(view);
	DuckCD.p = Math::Transpose(projection);
	DuckCD.l = mDirectionalLight;
	DuckCD.m = mMaterial;
	DuckCD.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };
	mDuckVertexShader.Bind();
	mDuckPixelShader.Bind();
	mDuckConstantBuffer.Set(&DuckCD);
	mDuckConstantBuffer.BindVS();
	mDuckConstantBuffer.BindPS();
	mDuckTexture.BindPS();
	mSampler.BindPS();
	mDuckMeshBuffer.Render();
	
	Graphics::SimpleDraw::Render(mCamera);

	{
		Graphics::Gui::BeginRender();
		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("FPS: %f", 1.0f / deltaTime);
		Graphics::Gui::EndRender();
	}
	Graphics::GraphicsSystem::Get()->EndRender();
}