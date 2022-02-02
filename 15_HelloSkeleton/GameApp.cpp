#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	bool stateMode{ false };
	bool playAnim{ false };
	bool cloak{ false };
	bool follow{ true };
	float cloakSize{ 10.0f };
	float timeLine = 0.0f;
	Math::Vector3 mPosition{ 0.0f,150.0f,0.0f };
	Math::Vector3 mModelPosition{ 0.0f,0.0f,0.0f };
	uint32_t rows{ 10u };
	uint32_t columns{ 17u };
	Math::Vector3 toModelPosition[9]
	{
		{1.0f,10.5f,-6.0f},
		{2.5f,7.6f,5.5f},
		{7.2f,-5.0f,10.0f},
		{0.0f,4.5f,15.0f},
		{0.0f,17.5f,17.5f},
		{0.0f,4.5f,15.5f},
		{-7.5f,-5.0f,10.0f},
		{-2.5f,9.2f,5.5f},
		{-3.0f,10.5f,-6.0f}
	};
	Math::Vector3 cloakPositions[9];
	struct ConstantData
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
	float GetRandomFloat(float low, float high)
	{
		return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
	}
	Math::Vector3 GetCloakPosition(const Math::Matrix4& mat, Math::Vector3 dir)
	{
		Math::Matrix4 temp = Math::Translation(dir)*mat;

		Graphics::SimpleDraw::AddLine({ mat.v4.x,mat.v4.y,mat.v4.z }, { temp.v4.x,temp.v4.y,temp.v4.z }, Math::Vector4::Cyan());
		return { temp.v4.x,temp.v4.y,temp.v4.z };
	}
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

	mCamera.SetPosition({ 0.0f,200.0f,-200.0f });
	mCamera.SetLookAt({ 0.0f,100.0f,5.0f });

	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/skybox01.png", 1000.0f);

	mState.Initialize(Graphics::RasterizerState::CullMode::None, Graphics::RasterizerState::FillMode::Solid);

	mModel.Load("../Assets/Models/Archer/Archer.txt", L"../Assets/Shaders/BoneShading.fx");

	mBoneConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(ConstantData)));
	mBoneCB.Initialize(static_cast<uint32_t>(sizeof(BoneData)));

	// Physics
	mSettings.drag = 0.005f;
	mSettings.iterations = 1u;
	mSettings.gravity = { 0.0f,-98.0f,0.0f };
	physicsWorld.Initialize(mSettings);
	physicsWorld.AddOBB(mOBBs[0]);

	// Cloak
	Graphics::MeshBuilder::GeneratePlane(mMesh, rows, columns);
	mMeshBuffer.Initialize(mMesh, true);
	mTexture.Initialize("../Assets/Images/Flag_AttackOnTitan_Green.png");
	mVertexShader.Initialize(L"../Assets/Shaders/Tank.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Tank.fx");
	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(ConstantData)));
}

void GameApp::OnTerminate()
{
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTexture.Terminate();
	mMeshBuffer.Terminate();
	mMesh.Destroy();

	mBoneCB.Terminate();
	mBoneConstantBuffer.Terminate();

	mModel.Unload();

	mState.Terminate();

	mSkyBox.Terminate();

	mSampler.Terminate();

	Graphics::SimpleDraw::Terminate();
	Graphics::Gui::Terminate();
	Graphics::SimpleDraw::Terminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate(bool(*GameLoop)(float))
{
	// deltaTime
	static auto prevTime = std::chrono::high_resolution_clock::now();
	auto currTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count() / 1000.0f;
	prevTime = currTime;

	Input();

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
	if (playAnim)
	{
		timeLine += deltaTime;
		mModelPosition += {0.0f, 0.0f, -1.2f};
	}

	physicsWorld.Update(deltaTime);

	if (follow&&playAnim)
	{
		mCamera.SetPosition(mCamera.GetPosition() + Math::Vector3{0.0f, 0.0f, -1.2f});
	}

	// Render
	Graphics::GraphicsSystem::Get()->BeginRender();

	mSkyBox.Render(mCamera);
	if (stateMode)
		physicsWorld.DebugDraw();

	// Cloak
	if (cloak)
	{
		for (uint32_t i = 0; i < columns; ++i)
		{
			//physicsWorld.GetParticles()[i]->position = { mPosition.x + (columns - static_cast<float>(i) - 1)*cloakSize,mPosition.y,mPosition.z };
			physicsWorld.GetParticles()[i]->position = 
			{ cloakPositions[i / 2].x + mModelPosition.x,
				cloakPositions[i / 2].y + mModelPosition.y,
				cloakPositions[i / 2].z + mModelPosition.z };
		}
		for (uint32_t i = 0; i < physicsWorld.GetParticles().size(); ++i)
		{
			mMesh.GetVertex(i).position = physicsWorld.GetParticles()[i]->position;
		}
		mMeshBuffer.UpdateVertices(mMesh.GetVertices(), mMesh.GetVertexCount());

		Math::Matrix4 world, view, projection;

		view = mCamera.GetViewMatrix();
		projection = mCamera.GetPerspectiveMatrix();

		ConstantData cd;
		cd.w = Math::Transpose(world);
		cd.v = Math::Transpose(view);
		cd.p = Math::Transpose(projection);
		cd.l = mLight;
		cd.m = mMaterial;
		cd.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };

		mConstantBuffer.Set(&cd);
		mConstantBuffer.BindVS();
		mConstantBuffer.BindPS();

		mSampler.BindPS();
		mTexture.BindVS(1);
		mTexture.BindPS(1);
		mPixelShader.Bind();
		mVertexShader.Bind();

		mMeshBuffer.Render();
	}

	// Model
	Math::Matrix4 world, view, projection;
	view = mCamera.GetViewMatrix();
	projection = mCamera.GetPerspectiveMatrix();
	ConstantData constantData;
	world = Math::Translation(mModelPosition);

	constantData.w = Math::Transpose(world);
	constantData.v = Math::Transpose(view);
	constantData.p = Math::Transpose(projection);
	constantData.l = mLight;
	constantData.m = mMaterial;
	constantData.vp = { mCamera.GetPosition().x,mCamera.GetPosition().y,mCamera.GetPosition().z,0.0f };

	mBoneConstantBuffer.Set(&constantData);
	mBoneConstantBuffer.BindVS();
	mBoneConstantBuffer.BindPS();

	// Bones
	GetBoneMatrices(mModel.root, timeLine);
	DrawBone(mModel.root);

	for (uint32_t i = 0; i < 9; ++i)
	{
		Graphics::SimpleDraw::AddSphere({ cloakPositions[i],1.0f }, Math::Vector4::Cyan());
	}
	/*for (uint32_t i = 0; i < mModel.bones.size(); ++i)
	{
		boneCD.bones[i] = Math::Transpose(boneCD.bones[i]);
	}*/
	mSampler.BindPS();
	mModel.Render(&boneCD);


	for (int i = -10; i < 11; ++i)
	{
		Graphics::SimpleDraw::AddLine({ -10.0f,0.0f,static_cast<float>(i) }, { 10.0f,0.0f,static_cast<float>(i) }, Math::Vector4::White());
		Graphics::SimpleDraw::AddLine({ static_cast<float>(i),0.0f,-10.0f }, { static_cast<float>(i),0.0f,10.0f }, Math::Vector4::White());
	}
	Graphics::SimpleDraw::Render(mCamera);

	{
		Graphics::Gui::BeginRender();
		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("FPS: %f", 1.0f / deltaTime);
		ImGui::Text("%f", mCubePosition.x);
		ImGui::Checkbox("Show Edge:", &stateMode);
		ImGui::Checkbox("Play Animation:", &playAnim);
		//ImGui::Checkbox("Cloak:", &cloak);
		ImGui::SliderFloat3("Position Check:", toModelPosition[8].f, -20.0f, 20.0f);

		ImGui::SliderFloat3("Light Direction:", mLight.mDirection.f, -1.0f, 1.0f);
		ImGui::SliderFloat4("Light Ambient:", mLight.mAmbient.f, 0.0f, 1.0f);
		ImGui::SliderFloat4("Light Diffuse:", mLight.mDiffuse.f, 0.0f, 1.0f);
		ImGui::SliderFloat4("Light Specular:", mLight.mSpecular.f, 0.0f, 1.0f);
		ImGui::SliderFloat4("Material Ambient:", mMaterial.mAmbient.f, 0.0f, 1.0f);
		ImGui::SliderFloat4("Material Diffuse:", mMaterial.mDiffuse.f, 0.0f, 1.0f);
		ImGui::SliderFloat4("Material Specular:", mMaterial.mSpecular.f, 0.0f, 1.0f);
		ImGui::SliderFloat("Material Power:", &mMaterial.mPower, 0.0f, 20.0f);

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
			boneCD.bones[bone->index] = boneCD.bones[bone->index]* Math::Inverse(bone->parent->offsetTransform) * boneCD.bones[bone->parentIndex];
		}
	}
	// Cloak
	switch (bone->index)
	{
	case 7:
		cloakPositions[5] = GetCloakPosition(Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[5]);
		break;
	case 8:
		cloakPositions[6] = GetCloakPosition( Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[6]);
		cloakPositions[7] = GetCloakPosition( Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[7]);
		cloakPositions[8] = GetCloakPosition( Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[8]);
		break;
	case 13:
		cloakPositions[3] = GetCloakPosition(Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[3]);

		break;
	case 14:
		cloakPositions[0] = GetCloakPosition( Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[0]);
		cloakPositions[1] = GetCloakPosition( Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[1]);
		cloakPositions[2] = GetCloakPosition( Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[2]);
		break;
	case 4:
		cloakPositions[4] = GetCloakPosition(Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index], toModelPosition[4]);
		break;
	default:
		break;
	}
	for (auto& child : bone->children)
	{
		GetBoneMatrices(child, deltaTime);
	}
}

void GameApp::DrawBone(S::Graphics::Bone* bone)
{
	Math::Matrix4 mat = Math::Inverse(bone->offsetTransform)*boneCD.bones[bone->index];
	if (bone->index == 13 || bone->index == 7 || bone->index == 8 || bone->index == 14)
	{
		Graphics::SimpleDraw::AddSphere(
			{ mat._4[0],
			mat._4[1],
			mat._4[2], 2.0f },
			Math::Vector4::LightBlue());
	}
	else if (bone->index == 3)
	{
		physicsWorld.GetOBBs()[0] = { mat._4[0] + mModelPosition.x,mat._4[1] + mModelPosition.y,mat._4[2] + mModelPosition.z,20.0f,40.0f,15.0f,{0.0f,0.0f,0.0f,1.0f} };
	}
	else
	{
		Graphics::SimpleDraw::AddSphere(
			{ mat._4[0],
			mat._4[1],
			mat._4[2], 2.0f },
			Math::Vector4::Yellow());
	}

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

void GameApp::Input()
{
	auto is = Input::InputSystem::Get();
	is->Update();
	{
		float move = 1.0f;
		// Move Camera
		if (is->IsKeyDown(Input::KeyCode::W))
		{
			mCamera.Walk(move);
		}
		if (is->IsKeyDown(Input::KeyCode::S))
		{
			mCamera.Walk(-move);
		}
		if (is->IsKeyDown(Input::KeyCode::A))
		{
			mCamera.Strafe(-move);
		}
		if (is->IsKeyDown(Input::KeyCode::D))
		{
			mCamera.Strafe(move);
		}
		if (is->IsKeyDown(Input::KeyCode::SPACE))
		{
			mCamera.Rise(move);
		}
		if (is->IsKeyDown(Input::KeyCode::C))
		{
			mCamera.Rise(-move);
		}
		if (is->IsKeyPressed(Input::KeyCode::F))
		{
			follow = !follow;
		}
		if (is->IsMouseDown(Input::MouseButton::RBUTTON))
		{
			mCamera.Yaw(is->GetMouseMoveX()*0.002f);
			mCamera.Pitch(is->GetMouseMoveY()*0.002f);
		}
		if (is->IsKeyPressed(Input::KeyCode::N))
		{
			cloak = true;
			physicsWorld.ClearDynamic();
			GenerateCloak(cloakPositions, rows, columns, cloakSize);
		}
	}
}

void GameApp::GenerateCloak(Math::Vector3 pos[], uint32_t rows, uint32_t columns, float restLength)
{
	for (uint32_t r = 0; r < rows; ++r)
	{
		for (int c = columns - 1; c >= 0; --c)
		{
			Physics::Particle* p = new Physics::Particle();
			if (r == 0)
			{
				p->SetPosition({ pos[(columns - c) / 2].x,pos[(columns - c) / 2].y,pos[(columns - c) / 2].z });
				p->SetVelocity({ 0.0f,0.0f,0.0f });
				p->sleep = true;
				p->radius = 0.2f;
				p->recipMass = 0.0f;
			}
			else if (r == rows - 1)
			{
				p->SetPosition({ pos[(columns - c) / 2].x - static_cast<float>(c % 2)*restLength,pos[(columns - c) / 2].y - static_cast<float>(r % 2)*restLength,pos[(columns - c) / 2].z });
				p->SetVelocity({ GetRandomFloat(-0.5f,0.5f),GetRandomFloat(-0.5f,0.5f),-10.0f });
				p->radius = 0.2f;
				p->recipMass = 1.0f;
			}
			else
			{
				p->SetPosition({ pos[(columns - c) / 2].x - static_cast<float>(c % 2)*restLength,pos[(columns - c) / 2].y - static_cast<float>(r % 2)*restLength,pos[(columns - c) / 2].z });
				//p->SetPosition({ static_cast<float>(c)*restLength + pos.x,pos.y - static_cast<float>(r % 2)*restLength,0.0f });
				p->SetVelocity({ 0.0f,0.0f,0.0f });
				p->radius = 0.2f;
				p->recipMass = 1.0f;
			}

			physicsWorld.AddParticle(p);
		}
	}
	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			if (r != 0)
			{
				Physics::Spring* springR = new Physics::Spring(physicsWorld.GetParticles()[c + columns * r],
					physicsWorld.GetParticles()[c + 1u + columns * r], restLength);
				physicsWorld.AddConstraint(springR);
			}

			Physics::Spring* springC = new Physics::Spring(physicsWorld.GetParticles()[c + columns * r],
				physicsWorld.GetParticles()[c + columns * (r + 1u)], restLength);
			physicsWorld.AddConstraint(springC);
		}
		Physics::Spring* spring = new Physics::Spring(physicsWorld.GetParticles()[columns - 1u + columns * r],
			physicsWorld.GetParticles()[columns - 1u + columns * (r + 1)], restLength);
		physicsWorld.AddConstraint(spring);
	}
	for (uint32_t i = 0; i < columns - 1; ++i)
	{
		Physics::Spring* spring = new Physics::Spring(physicsWorld.GetParticles()[i + columns * (rows - 1u)],
			physicsWorld.GetParticles()[i + 1u + columns * (rows - 1u)], restLength);
		physicsWorld.AddConstraint(spring);
	}
}