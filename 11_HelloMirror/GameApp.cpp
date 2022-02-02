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
	mWindow.Initialize(GetInstance(), "Mirror", 1280, 720); //1280
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Graphics::SimpleDraw::Initialize(1000000);
	Graphics::Gui::Initialize(mWindow.GetWindowHandle());

	//Graphics::MeshBuilder::GenerateSphere(mSphereMesh, 256u, 129u);
	mModelLoader.LoadOBJVertex("../Assets/Models/Duck/duck.txt", 0.05f, mSphereMesh);
	mSphereMeshBuffer.Initialize(mSphereMesh);

	mNormalTexture.Initialize("../Assets/Images/earth_normal.jpg");
	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Clamp);
	mSphereConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mSphereVertexShader.Initialize(L"../Assets/Shaders/SphereReflection.fx", Graphics::Vertex::Format);
	mSpherePixelShader.Initialize(L"../Assets/Shaders/SphereReflection.fx");

	mCamera.SetPosition({ 20.0f,30.0f,0.0f });
	mCamera.SetLookAt({ 20.0f,30.0f,50.0f });

	for (int i = 0; i < 6; i++)
	{
		mSubCamera[i].SetPosition(mSpherePosition);
		mSubCamera[i].SetFOV(90.0f*Math::kDegToRad);
	}
	mSubCamera[0].SetDirection(Math::Normalize({ 0.0f,0.999f,0.001f }));	// Up
	mSubCamera[1].SetDirection(Math::Normalize({ 0.0f,-0.999f,0.001f }));	// Down
	mSubCamera[2].SetDirection({ -1.0f,0.0f,0.0f });						// Left
	mSubCamera[3].SetDirection({ 1.0f,0.0f,0.0f });							// Right
	mSubCamera[4].SetDirection({ 0.0f,0.0f,-1.0f });						// Front
	mSubCamera[5].SetDirection({ 0.0f,0.0f,1.0f });							// Back

	for (int i = 0; i < 6; ++i)
	{
		mCubeRenderTarget[i].Initialize();
	}

	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/skybox01.png", 1000.0f);
	mTerrain.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/Sand.jpg", "../Assets/Images/BlueCircle.png", 50u, 40u, 20u);

	// Tank
	mModelLoader.LoadOBJ("../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(sizeof(S::Graphics::Vertex), mTankMesh.GetVertices(), mTankMesh.GetVertexCount(), mTankMesh.GetIndices(), mTankMesh.GetIndexCount());
	mTankTexture[0].Initialize("../Assets/Models/Tank/tank_ao.jpg");
	mTankTexture[1].Initialize("../Assets/Models/Tank/tank_diffuse.jpg");
	mTankTexture[2].Initialize("../Assets/Models/Tank/tank_specular.jpg");
	mTankVertexShader.Initialize(L"../Assets/Shaders/Tank.fx", Graphics::Vertex::Format);
	mTankPixelShader.Initialize(L"../Assets/Shaders/Tank.fx");
	mTankConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(TankCData)));

	// Cube
	Graphics::MeshBuilder::GenerateSphereByCube(mCubeMesh);
	mCubeMeshBuffer.Initialize(mCubeMesh);
	mCubeVertexShader.Initialize(L"../Assets/Shaders/CubeToSphere.fx", Graphics::Vertex::Format);
	mCubePixelShader.Initialize(L"../Assets/Shaders/CubeToSphere.fx");
	mCubeConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mModel.Load("../Assets/Models/Archer/Archer.txt", L"../Assets/Shaders/BoneShading.fx");
	mState.Initialize(Graphics::RasterizerState::CullMode::None, Graphics::RasterizerState::FillMode::Wireframe);
	mBoneConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(TankCData)));
	mBoneCB.Initialize(static_cast<uint32_t>(sizeof(BoneData)));
}

void GameApp::OnTerminate()
{
	mBoneCB.Terminate();
	mBoneConstantBuffer.Terminate();
	mModel.Unload();
	mState.Terminate();

	mCubeConstantBuffer.Terminate();
	mCubePixelShader.Terminate();
	mCubeVertexShader.Terminate();
	mCubeMeshBuffer.Terminate();
	mCubeMesh.Destroy();

	mTankConstantBuffer.Terminate();

	mTankPixelShader.Terminate();
	mTankVertexShader.Terminate();
	for (int i = 0; i < 3; ++i)
	{
		mTankTexture[i].Terminate();
	}
	mTankMeshBuffer.Terminate();
	mTankMesh.Destroy();


	mTerrain.Terminate();
	mSkyBox.Terminate();

	for (int i = 0; i < 6; ++i)
	{
		mCubeRenderTarget[i].Terminate();
	}
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

void GameApp::OnUpdate(bool(*GameLoop)(float))
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
			if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::R))
			{
				mCamera.SetPosition(mTankPosition);
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

	for (int i = 0; i < 6; i++)
	{
		mSubCamera[i].SetPosition(mSpherePosition);
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

	// Model
	Math::Matrix4 w, v, p;
	v = mCamera.GetViewMatrix();
	p = mCamera.GetPerspectiveMatrix();
	TankCData constantData;

	constantData.w = Math::Transpose(w);
	constantData.v = Math::Transpose(v);
	constantData.p = Math::Transpose(p);
	constantData.l = mDirectionalLight;
	constantData.m = mMaterial;
	constantData.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };

	mBoneConstantBuffer.Set(&constantData);
	mBoneConstantBuffer.BindVS();
	mBoneConstantBuffer.BindPS();

	// Bones
	GetBoneMatrices(mModel.root, timeLine);
	DrawBone(mModel.root);

	mSampler.BindPS();
	mModel.Render(&boneCD);

	// Tank
	Math::Matrix4 world, view, projection, lightview, lightprojection;
	/*view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();
	TankCData tankCD;
	world = Math::Translation(mTankPosition);
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
	mTankMeshBuffer.Render();*/

	if (drawDuck)
	{
		// SubCameras
		for (int i = 0; i < 6; ++i)
		{
			mCubeRenderTarget[i].BeginRender();

			mTerrain.Render(mSubCamera[i], 1.0f);
			mSkyBox.Render(mSubCamera[i], 1.0f);

			Math::Matrix4 world, view, projection;
			view = mSubCamera[i].GetViewMatrix();
			projection = mSubCamera[i].GetPerspectiveMatrix(1.0f);

			// Model
			TankCData constantData;

			constantData.w = Math::Transpose(world);
			constantData.v = Math::Transpose(view);
			constantData.p = Math::Transpose(projection);
			constantData.l = mDirectionalLight;
			constantData.m = mMaterial;
			constantData.vp = { mSubCamera[i].GetPosition().x,mSubCamera[i].GetPosition().y,mSubCamera[i].GetPosition().z,0.0f };

			mBoneConstantBuffer.Set(&constantData);
			mBoneConstantBuffer.BindVS();
			mBoneConstantBuffer.BindPS();

			mSampler.BindPS();
			mModel.Render(&boneCD);

			// Tank
			/*TankCData tankCD;
			world = Math::Translation(mTankPosition);
			tankCD.w = Math::Transpose(world);
			tankCD.v = Math::Transpose(view);
			tankCD.p = Math::Transpose(projection);
			tankCD.l = mDirectionalLight;
			tankCD.m = mMaterial;
			tankCD.vp = { mSubCamera[i].GetPosition().x,mSubCamera[i].GetPosition().y,mSubCamera[i].GetPosition().z,0.0f };
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
			mTankMeshBuffer.Render();*/

			mCubeRenderTarget[i].EndRender();
		}

		/*{
			view = mCamera.GetViewMatrix();
			projection = mCamera.GetPerspectiveMatrix();
			CData cCD;
			world = Math::Matrix4::Scaling(10.0f)*Math::Translation(mCubePosition);
			cCD.w = Math::Transpose(world);
			cCD.v = Math::Transpose(view);
			cCD.p = Math::Transpose(projection);
			cCD.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };
			mCubeConstantBuffer.Set(&cCD);
			mCubeConstantBuffer.BindVS();
			mCubeConstantBuffer.BindPS();
			mCubeVertexShader.Bind();
			mCubePixelShader.Bind();
			for (uint32_t i = 0; i < 6; ++i)
			{
				mCubeRenderTarget[i].BindPS(i);
			}
			mSampler.BindPS();

			mCubeMeshBuffer.Render();
			for (uint32_t i = 0; i < 6; ++i)
			{
				mCubeRenderTarget[i].UnbindPS(i);
			}
		}*/


		mSphereVertexShader.Bind();
		mSpherePixelShader.Bind();

		world = Math::Matrix4::Scaling(10.0f)*Math::Translation(mSpherePosition);
		view = mCamera.GetViewMatrix();
		projection = mCamera.GetPerspectiveMatrix();

		CData cd;
		cd.w = Math::Transpose(world);
		cd.v = Math::Transpose(view);
		cd.p = Math::Transpose(projection);
		cd.vp = { mCamera.GetPosition(),0.0f };
		mSphereConstantBuffer.Set(&cd);
		mSphereConstantBuffer.BindVS();
		mSphereConstantBuffer.BindPS();
		for (uint32_t i = 0; i < 6; ++i)
		{
			mCubeRenderTarget[i].BindPS(i);
		}
		mNormalTexture.BindPS(6);
		mSampler.BindPS();
		mSphereMeshBuffer.Render();
		for (uint32_t i = 0; i < 6; ++i)
		{
			mCubeRenderTarget[i].UnbindPS(i);
		}
	}
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

void GameApp::GetBoneMatrices(Graphics::Bone* bone, float deltaTime)
{
	if (!mModel.animations[0].GetTransform(deltaTime, bone->index, boneCD.bones[bone->index]))
	{
		boneCD.bones[bone->index] = bone->toParentTransform;
	}
	boneCD.bones[bone->index] = bone->offsetTransform * boneCD.bones[bone->index];
	if (bone->parent)
	{
		if (mModel.animations[0].boneAnimations[bone->parentIndex])
		{
			boneCD.bones[bone->index] = boneCD.bones[bone->index] * Math::Inverse(bone->parent->offsetTransform) * boneCD.bones[bone->parentIndex];
		}
	}
	for (auto& child : bone->children)
	{
		GetBoneMatrices(child, deltaTime);
	}
}

void GameApp::DrawBone(S::Graphics::Bone* bone)
{
	Math::Matrix4 mat = Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index];
	Graphics::SimpleDraw::AddSphere(
		{ mat._4[0],
		mat._4[1],
		mat._4[2], 2.0f },
		Math::Vector4::Yellow());

	for (auto& child : bone->children)
	{
		Math::Matrix4 matC = Math::Inverse(child->offsetTransform)*boneCD.bones[child->index];

		Graphics::SimpleDraw::AddLine(
			{ mat._4[0],
			mat._4[1],
			mat._4[2] },
			{ matC._4[0],
			matC._4[1],
			matC._4[2] },
			Math::Vector4::Magenta());
		DrawBone(child);
	}
}