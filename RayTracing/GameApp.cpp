#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	bool stateMode{ false };
	bool playAnim{ false };
	bool drawDuck{ false };
	float timeLine = 0.0f;

	float fov = 90.0f;
	Math::Vector3 v{ 1.0f,1.0f,1.0f };
	Math::Vector3 vt = Math::Normalize(v);
	struct CData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
		Math::Vector4 vp;
	};
	struct TankCData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
		Graphics::DirectionalLight l;
		Graphics::Material m;
		Math::Vector4 vp;
	};
	struct BoneData
	{
		Math::Matrix4 bones[100];
	};
	BoneData boneCD;
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

	Graphics::MeshBuilder::GenerateSphere(mSphereMesh, 256u, 129u);
	//mModelLoader.LoadOBJVertex("../Assets/Models/Duck/duck.txt", 0.05f, mSphereMesh);
	mSphereMeshBuffer.Initialize(mSphereMesh);

	mNormalTexture.Initialize("../Assets/Images/earth_normal.jpg");
	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Clamp);
	mSphereConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mSphereVertexShader.Initialize(L"../Assets/Shaders/SphereReflection.fx", Graphics::Vertex::Format);
	mSpherePixelShader.Initialize(L"../Assets/Shaders/SphereReflection.fx");

	mCamera.SetPosition({ 20.0f,30.0f,0.0f });
	mCamera.SetLookAt({ 20.0f,30.0f,50.0f });

	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/skybox01.png", 1000.0f);
	mTerrain.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/Sand.jpg", "../Assets/Images/BlueCircle.png", 50u, 40u, 20u);

	// Cube
	Graphics::MeshBuilder::GenerateSphereByCube(mCubeMesh);
	mCubeMeshBuffer.Initialize(mCubeMesh);
	mCubeVertexShader.Initialize(L"../Assets/Shaders/CubeToSphere.fx", Graphics::Vertex::Format);
	mCubePixelShader.Initialize(L"../Assets/Shaders/CubeToSphere.fx");
	mCubeConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));
	
	mState.Initialize(Graphics::RasterizerState::CullMode::None, Graphics::RasterizerState::FillMode::Wireframe);
}

void GameApp::OnTerminate()
{
	mState.Terminate();

	mCubeConstantBuffer.Terminate();
	mCubePixelShader.Terminate();
	mCubeVertexShader.Terminate();
	mCubeMeshBuffer.Terminate();
	mCubeMesh.Destroy();

	mTerrain.Terminate();
	mSkyBox.Terminate();

	mSpherePixelShader.Terminate();
	mSphereVertexShader.Terminate();
	mSphereConstantBuffer.Terminate();
	mSampler.Terminate();
	mNormalTexture.Terminate();
	mSphereMeshBuffer.Terminate();
	mSphereMesh.Destroy();

	Graphics::SimpleDraw::Terminate();
	Graphics::Gui::Terminate();
	Graphics::SimpleDraw::Terminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	{
		Input::InputSystem::Get()->Update();
		{
			float move = 1.0f;
			// Move Camera
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::W))
			{
				mCamera.Walk(move);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::S))
			{
				mCamera.Walk(-move);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::A))
			{
				mCamera.Strafe(-move);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::D))
			{
				mCamera.Strafe(move);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::SPACE))
			{
				mCamera.Rise(move);
			}
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::C))
			{
				mCamera.Rise(-move);
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

	stateMode ? mState.Set() : mState.Clear();
	if (playAnim)
	{
		timeLine += deltaTime;
	}

	// Render
	Graphics::GraphicsSystem::Get()->BeginRender();

	mSkyBox.Render(mCamera);
	mTerrain.Render(mCamera);



	Graphics::SimpleDraw::AddSphere(mSpherePosition, 5.0f, Math::Vector4::Yellow());

	Graphics::SimpleDraw::Render(mCamera);

	{
		Graphics::Gui::BeginRender();
		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::SliderFloat("x", &mSpherePosition.x, -100.0f, 100.0f);
		ImGui::SliderFloat("y", &mSpherePosition.y, -100.0f, 100.0f);
		ImGui::SliderFloat("z", &mSpherePosition.z, -100.0f, 100.0f);
		ImGui::Text("FPS: %f", 1.0f / deltaTime);

		ImGui::Checkbox("Show Edge:", &stateMode);
		ImGui::Checkbox("Play Animation:", &playAnim);
		ImGui::Checkbox("Dynamic Environment Reflection:", &drawDuck);
		//ImGui::SliderFloat("FOV", &fov, 10.0f, 180.0f);
		//mCamera.SetFOV(fov);

		/*ImGui::SliderFloat("nx", &v.x, -1.0f, 1.0f);
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
