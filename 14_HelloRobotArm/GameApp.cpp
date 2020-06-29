#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	bool stateMode{ true };
	uint32_t test = 0;
	float timeLine = 0.0f;
	struct CubeCData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
	};
	struct CylinderCData
	{
		Math::Matrix4 w;
		Math::Matrix4 v;
		Math::Matrix4 p;
		Graphics::DirectionalLight l;
		Graphics::Material m;
		Math::Vector4 vp;
		Math::Matrix4 bone[3];
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

	mCamera.SetPosition({ 2.5f,7.0f,-10.0f });
	mCamera.SetLookAt({ 2.5f,5.0f,5.0f });

	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/skybox01.png", 1000.0f);

	mState.Initialize(Graphics::RasterizerState::CullMode::None, Graphics::RasterizerState::FillMode::Wireframe);

	// Cube
	Graphics::MeshBuilder::GenerateCube(mCubeMesh);
	mCubeMeshBuffer.Initialize(sizeof(Graphics::Vertex), mCubeMesh.GetVertices(), mCubeMesh.GetVertexCount(), mCubeMesh.GetIndices(), mCubeMesh.GetIndexCount());
	mCubeTexture.Initialize("../Assets/Images/letter_S.png");
	mCubeVertexShader.Initialize(L"../Assets/Shaders/SkyBox.fx", Graphics::Vertex::Format);
	mCubePixelShader.Initialize(L"../Assets/Shaders/SkyBox.fx");
	mCubeConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CubeCData)));

	// BoneMesh
	Graphics::MeshBuilder::GenerateCylinder(mBoneMesh, 6.0f, 1.0f, 32u, 65u);
	mBoneMeshBuffer.Initialize(sizeof(Graphics::BoneVertex), mBoneMesh.GetVertices(), mBoneMesh.GetVertexCount(), mBoneMesh.GetIndices(), mBoneMesh.GetIndexCount());
	mBoneVertexShader.Initialize(L"../Assets/Shaders/Cylinder.fx", Graphics::BoneVertex::Format);
	mBonePixelShader.Initialize(L"../Assets/Shaders/Cylinder.fx");
	mBoneConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CylinderCData)));

	Math::Quaternion quat = Math::QuaternionRotationAxis(Math::Vector3::ZAxis(), 0.03f);
	mQuaternion[0] = mQuaternion[0] * quat;
	mQuaternion[1] = { -mQuaternion[0].x,-mQuaternion[0].y,-mQuaternion[0].z,mQuaternion[0].w };

	for (int i = 0; i < 3; i++)
	{
		Bone *bone = new Bone();
		//bone->transform = Math::Matrix4::Translation({ 0.0f,2.0f,0.0f });
		mBones.emplace_back(bone);
	}
	mBones[0]->transform = Math::Matrix4::Identity()*Math::Matrix4::Translation({ 0.0,1.0f,0.0 });
	for (int i = 1; i < 3; ++i)
	{
		mBones[i]->parent = mBones[i - 1];
	}

	toParentTransform = Math::Translation({ 0.0f,2.0f,0.0f });
}

void GameApp::OnTerminate()
{
	mBoneConstantBuffer.Terminate();

	mBonePixelShader.Terminate();
	mBoneVertexShader.Terminate();

	mBoneMeshBuffer.Terminate();
	mBoneMesh.Destroy();

	mCubeConstantBuffer.Terminate();

	mCubePixelShader.Terminate();
	mCubeVertexShader.Terminate();
	
	mCubeTexture.Terminate();
	mCubeMeshBuffer.Terminate();
	mCubeMesh.Destroy();

	mState.Terminate();

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

			if (is->IsKeyPressed(Input::KeyCode::ONE))
			{
				test = 0;
			}
			if (is->IsKeyPressed(Input::KeyCode::TWO))
			{
				test = 1;
			}
			if (is->IsKeyPressed(Input::KeyCode::THREE))
			{
				test = 2;
			}

			if (is->IsKeyDown(Input::KeyCode::LEFT))
			{
				mRotateMatrix = Math::MatrixRotationQuaternion(mQuaternion[0]);
				mBones[test]->transform = mRotateMatrix * mBones[test]->transform;
			}
			if (is->IsKeyDown(Input::KeyCode::RIGHT))
			{
				mRotateMatrix = Math::MatrixRotationQuaternion(mQuaternion[1]);
				mBones[test]->transform = mRotateMatrix * mBones[test]->transform;
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

	stateMode ? mState.Set() : mState.Clear();

	// Render
	Graphics::GraphicsSystem::Get()->BeginRender();

	mSkyBox.Render(mCamera);

	// Cube
	Math::Matrix4 world, view, projection;
	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();
	CubeCData CubeCD;

	for (int i = 0; i < 3; ++i)
	{
		Bone* bone = mBones[i];
		world = Math::Translation({ 0.0f,1.0f,0.0f }) * bone->transform;
		while(bone->parent != nullptr)
		{
			bone = bone->parent;
			world = world * Math::Matrix4::Translation({ 0.0f,2.0f,0.0f })* bone->transform;
		}

		CubeCD.w = Math::Transpose(world);
		CubeCD.v = Math::Transpose(view);
		CubeCD.p = Math::Transpose(projection);
		mCubeVertexShader.Bind();
		mCubePixelShader.Bind();
		mCubeConstantBuffer.Set(&CubeCD);
		mCubeConstantBuffer.BindVS();
		mCubeConstantBuffer.BindPS();
		mCubeTexture.BindPS();
		mSampler.BindPS();
		mCubeMeshBuffer.Render();
	}

	// Cylinder
	world = Math::Translation(mBonePosition);
	CylinderCData cylinderCD;
	cylinderCD.w = Math::Transpose(world);
	cylinderCD.v = Math::Transpose(view);
	cylinderCD.p = Math::Transpose(projection);
	cylinderCD.l = mLight;
	cylinderCD.m = mMaterial;
	cylinderCD.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };
	for (int i = 0; i < 3; ++i)
	{
		Bone* bone = mBones[i];
		world = bone->transform;
		while (bone->parent != nullptr)
		{
			bone = bone->parent;
			world = Math::Translation({ 0.0f,-2.0f,0.0f })*world *toParentTransform* bone->transform;
		}
		cylinderCD.bone[i] = world;
	}

	mBoneVertexShader.Bind();
	mBonePixelShader.Bind();
	mBoneConstantBuffer.Set(&cylinderCD);
	mBoneConstantBuffer.BindVS();
	mBoneConstantBuffer.BindPS();
	mCubeTexture.BindPS();
	mSampler.BindPS();
	mBoneMeshBuffer.Render();

	Graphics::SimpleDraw::AddLine({ 0.0f,0.0f,0.0f }, { 10.0f,0.0f,0.0f }, Math::Vector4::Magenta());
	Graphics::SimpleDraw::Render(mCamera);

	{
		Graphics::Gui::BeginRender();
		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("FPS: %f", 1.0f / deltaTime);
		ImGui::Text("%f", mCubePosition.x);
		ImGui::Checkbox("Show Edge Only:", &stateMode);
		Graphics::Gui::EndRender();
	}
	Graphics::GraphicsSystem::Get()->EndRender();
}