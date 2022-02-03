#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	bool stateMode{ true };
	bool debugDraw{ true };
	bool flag{ false };
	bool pause{ false };
	Math::Vector3 mPosition{ 0.0f,10.0f,0.0f };
	uint32_t rows{ 22u };
	uint32_t columns{ 40u };
	float GetRandomFloat(float low, float high)
	{
		return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
	}
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
	mWindow.Initialize(GetInstance(), "Hello Physics", 1280, 720); //1280
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Graphics::SimpleDraw::Initialize(1000000);
	Graphics::Gui::Initialize(mWindow.GetWindowHandle());

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);

	mCamera.SetPosition({ 0.0f,20.0f,-20.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,1.0f });

	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/skybox01.png", 1000.0f);

	mState.Initialize(Graphics::RasterizerState::CullMode::None, Graphics::RasterizerState::FillMode::Solid);
	mSettings.drag = 0.005f;
	mSettings.iterations = 3u;
	physicsWorld.Initialize(mSettings);
	physicsWorld.AddPlane({ 0.0f,1.0f,0.0f,0.0f });
	physicsWorld.AddOBB(Geometry::OBB{ 0.0f,3.5f,0.0f,10.0f,3.0f,2.0f,{0.0f,0.0f,0.0f,1.0f } });

	// Cloak
	Graphics::MeshBuilder::GeneratePlane(mMesh, rows, columns);
	mMeshBuffer.Initialize(mMesh, true);
	mTexture.Initialize("../Assets/Images/Flag_AttackOnTitan_Green.png");
	mVertexShader.Initialize(L"../Assets/Shaders/Tank.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Tank.fx");
	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));
}

void GameApp::OnTerminate()
{
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mTexture.Terminate();
	mMeshBuffer.Terminate();
	mMesh.Destroy();

	physicsWorld.ClearDynamic();
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

	physicsWorld.Pause(pause);

	stateMode ? mState.Set() : mState.Clear();

	physicsWorld.Update(deltaTime);

	// Render
	Graphics::GraphicsSystem::Get()->BeginRender();
	if (debugDraw)
		physicsWorld.DebugDraw();
	mSkyBox.Render(mCamera);


	// Cloak
	if (flag)
	{
		for (uint32_t i = 0; i < columns; ++i)
		{
			physicsWorld.GetParticles()[i]->position = { mPosition.x + columns - static_cast<float>(i) - 1,mPosition.y,mPosition.z };
		}
		for (uint32_t i = 0; i < physicsWorld.GetParticles().size(); ++i)
		{
			mMesh.GetVertex(i).position = physicsWorld.GetParticles()[i]->position;
		}
		mMeshBuffer.UpdateVertices(mMesh.GetVertices(), mMesh.GetVertexCount());

		Math::Matrix4 world, view, projection;

		view = mCamera.GetViewMatrix();
		projection = mCamera.GetPerspectiveMatrix();

		CData cd;
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

	for (float i = -10; i < 11; i += 1.0f)
	{
		Graphics::SimpleDraw::AddLine({ -10.0f,0.0f,i }, { 10.0f,0.0f,i }, Math::Vector4::White());
		Graphics::SimpleDraw::AddLine({ i,0.0f,-10.0f }, { i,0.0f,10.0f }, Math::Vector4::White());
	}
	Graphics::SimpleDraw::AddTransform(Math::Matrix4::Identity());
	Graphics::SimpleDraw::Render(mCamera);

	{
		Graphics::Gui::BeginRender();
		ImGui::SetNextWindowPos({ 30.0f,30.0f });
		//ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("FPS: %f", 1.0f / deltaTime);
		ImGui::Checkbox("Show Edge Only:", &stateMode);
		ImGui::Checkbox("DebugDraw:", &debugDraw);
		ImGui::Checkbox("Pause:", &pause);
		ImGui::SliderFloat3("Flag's Position: ", mPosition.f, -10.0f, 10.0f);
		Graphics::Gui::EndRender();
	}
	Graphics::GraphicsSystem::Get()->EndRender();
}

void GameApp::Input()
{
	auto is = Input::InputSystem::Get();
	is->Update();
	{
		float move = 0.1f;
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
		if (is->IsKeyDown(Input::KeyCode::E))
		{
			mCamera.Yaw(0.01f);
		}
		if (is->IsKeyDown(Input::KeyCode::Q))
		{
			mCamera.Yaw(-0.01f);
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
		if (is->IsKeyDown(Input::KeyCode::TWO))
		{
			flag = false;
			physicsWorld.ClearDynamic();
			int numBox = 10;
			for (int j = 0; j < numBox; ++j)
			{
				Physics::Particle* p[8];
				for (int i = 0; i < 8; ++i)
				{
					p[i] = new Physics::Particle();
					p[i]->SetPosition({ 0.0f,0.0f,0.0f });
					p[i]->SetVelocity({ GetRandomFloat(-1.0f,1.0f),
									GetRandomFloat(0.0,1.0f),
									GetRandomFloat(-1.0f,1.0f) });
					p[i]->radius = 0.2f;
					p[i]->recipMass = 1.0f;
				}
				p[0]->SetPosition({ -1.0f,1.0f,1.0f });
				p[1]->SetPosition({ 1.0f,1.0f,1.0f });
				p[2]->SetPosition({ -1.0f,1.0f,-1.0f });
				p[3]->SetPosition({ 1.0f,1.0f,-1.0f });
				p[4]->SetPosition({ -1.0f,-1.0f,1.0f });
				p[5]->SetPosition({ 1.0f,-1.0f,1.0f });
				p[6]->SetPosition({ -1.0f,-1.0f,-1.0f });
				p[7]->SetPosition({ 1.0f,-1.0f,-1.0f });
				for (int i = 0; i < 8; ++i)
				{
					physicsWorld.AddParticle(p[i]);
				}

				Physics::Spring* sTFLTFR = new Physics::Spring(p[0], p[1], 1.0f);
				physicsWorld.AddConstraint(sTFLTFR);
				Physics::Spring* sTFLTBL = new Physics::Spring(p[0], p[2], 1.0f);
				physicsWorld.AddConstraint(sTFLTBL);
				Physics::Spring* sTRFTRB = new Physics::Spring(p[1], p[3], 1.0f);
				physicsWorld.AddConstraint(sTRFTRB);
				Physics::Spring* sTLBTRB = new Physics::Spring(p[2], p[3], 1.0f);
				physicsWorld.AddConstraint(sTLBTRB);

				Physics::Spring* sTFLBFL = new Physics::Spring(p[0], p[4], 1.0f);
				physicsWorld.AddConstraint(sTFLBFL);
				Physics::Spring* sTRFBRF = new Physics::Spring(p[1], p[5], 1.0f);
				physicsWorld.AddConstraint(sTRFBRF);
				Physics::Spring* s0 = new Physics::Spring(p[2], p[6], 1.0f);
				physicsWorld.AddConstraint(s0);
				Physics::Spring* sTRBBRB = new Physics::Spring(p[3], p[7], 1.0f);
				physicsWorld.AddConstraint(sTRBBRB);

				Physics::Spring* s1 = new Physics::Spring(p[4], p[6], 1.0f);
				physicsWorld.AddConstraint(s1);
				Physics::Spring* s2 = new Physics::Spring(p[7], p[6], 1.0f);
				physicsWorld.AddConstraint(s2);
				Physics::Spring* sBRFBLF = new Physics::Spring(p[5], p[4], 1.0f);
				physicsWorld.AddConstraint(sBRFBLF);
				Physics::Spring* sBRFBRB = new Physics::Spring(p[5], p[7], 1.0f);
				physicsWorld.AddConstraint(sBRFBRB);


				Physics::Spring* sTFLTBR = new Physics::Spring(p[0], p[3], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(sTFLTBR);
				Physics::Spring* s5 = new Physics::Spring(p[1], p[2], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s5);

				Physics::Spring* sTFLBFR = new Physics::Spring(p[0], p[5], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(sTFLBFR);
				Physics::Spring* s6 = new Physics::Spring(p[1], p[4], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s6);

				Physics::Spring* sTFLBLB = new Physics::Spring(p[0], p[6], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(sTFLBLB);
				Physics::Spring* s7 = new Physics::Spring(p[2], p[4], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s7);

				Physics::Spring* sTRFBRB = new Physics::Spring(p[1], p[7], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(sTRFBRB);
				Physics::Spring* s8 = new Physics::Spring(p[3], p[5], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s8);

				Physics::Spring* s3 = new Physics::Spring(p[3], p[6], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s3);
				Physics::Spring* s9 = new Physics::Spring(p[2], p[7], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s9);


				Physics::Spring* s4 = new Physics::Spring(p[5], p[6], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s4);
				Physics::Spring* s10 = new Physics::Spring(p[4], p[7], Math::Sqrt(2.0f));
				physicsWorld.AddConstraint(s10);
			}
		}
		if (is->IsKeyPressed(Input::KeyCode::ONE))
		{
			flag = false;
			physicsWorld.ClearDynamic();
			int evenNum = 33;
			for (int i = 0; i < evenNum; i += 3)
			{
				Physics::Particle* pA = new Physics::Particle();
				Physics::Particle* pB = new Physics::Particle();
				Physics::Particle* pC = new Physics::Particle();

				pA->SetPosition({ 0.0f,0.0f,0.0f });
				pA->SetVelocity({ GetRandomFloat(-0.1f,0.1f),
								GetRandomFloat(0.0,0.25f),
								GetRandomFloat(-0.1f,0.1f) });
				pA->radius = 0.2f;
				pA->recipMass = 1.0f;
				pA->bounce = 0.9f;
				physicsWorld.AddParticle(pA);

				pB->SetPosition({ 0.0f,0.0f,0.0f });
				pB->SetVelocity({ GetRandomFloat(-0.1f,0.1f),
								GetRandomFloat(0.0,0.25f),
								GetRandomFloat(-0.1f,0.1f) });
				pB->radius = 0.2f;
				pB->recipMass = 1.0f;
				pB->bounce = 0.9f;
				physicsWorld.AddParticle(pB);

				pC->SetPosition({ 0.0f,0.0f,0.0f });
				pC->SetVelocity({ GetRandomFloat(-0.1f,0.1f),
								GetRandomFloat(0.0,0.25f),
								GetRandomFloat(-0.1f,0.1f) });
				pC->radius = 0.2f;
				pC->recipMass = 1.0f;
				pC->bounce = 0.9f;
				physicsWorld.AddParticle(pC);

				Physics::Spring* spring1 = new Physics::Spring(pA, pB, 1.0f);
				physicsWorld.AddConstraint(spring1);
				Physics::Spring* spring2 = new Physics::Spring(pC, pB, 1.0f);
				physicsWorld.AddConstraint(spring2);
			}
		}
		if (is->IsKeyPressed(Input::KeyCode::THREE))
		{
			flag = true;
			physicsWorld.ClearDynamic();
			GenerateCloak({ 0.0f, 10.0f }, rows, columns);
		}
	}
}

void GameApp::GenerateCloak(Math::Vector2 pos, uint32_t rows, uint32_t columns)
{
	for (uint32_t r = 0; r < rows; ++r)
	{
		for (int c = columns - 1; c >= 0; --c)
		{
			Physics::Particle* p = new Physics::Particle();
			if (r == 0)
			{
				p->SetPosition({ static_cast<float>(c) + pos.x,pos.y,0.0f });
				p->SetVelocity({ 0.0f,0.0f,0.0f });
				p->sleep = true;
				p->radius = 0.2f;
				p->recipMass = 0.0f;
			}
			else if (r == rows - 1)
			{
				p->SetPosition({ static_cast<float>(c) + pos.x,pos.y - static_cast<float>(r % 2),0.0f });
				p->SetVelocity({ GetRandomFloat(-0.5f,0.5f),GetRandomFloat(-0.5f,0.5f),-3.0f });
				p->radius = 0.2f;
				p->recipMass = 1.0f;
				p->bounce = 0.9f;
			}
			else
			{
				p->SetPosition({ static_cast<float>(c) + pos.x,pos.y - static_cast<float>(r % 2),0.0f });
				p->SetVelocity({ 0.0f,0.0f,0.0f });
				p->radius = 0.2f;
				p->recipMass = 1.0f;
				p->bounce = 0.9f;
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
					physicsWorld.GetParticles()[c + 1u + columns * r], 1.0f);
				physicsWorld.AddConstraint(springR);
			}

			Physics::Spring* springC = new Physics::Spring(physicsWorld.GetParticles()[c + columns * r],
				physicsWorld.GetParticles()[c + columns * (r + 1u)], 1.0f);
			physicsWorld.AddConstraint(springC);
		}
		Physics::Spring* spring = new Physics::Spring(physicsWorld.GetParticles()[columns - 1u + columns * r],
			physicsWorld.GetParticles()[columns - 1u + columns * (r + 1)], 1.0f);
		physicsWorld.AddConstraint(spring);
	}
	for (uint32_t i = 0; i < columns - 1; ++i)
	{
		Physics::Spring* spring = new Physics::Spring(physicsWorld.GetParticles()[i + columns * (rows - 1u)],
			physicsWorld.GetParticles()[i + 1u + columns * (rows - 1u)], 1.0f);
		physicsWorld.AddConstraint(spring);
	}
}
