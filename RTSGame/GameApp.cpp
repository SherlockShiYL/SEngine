#include "GameApp.h"
#include <Input/Inc/Input.h>
#include <External/ImGui/Inc/imgui.h>

using namespace S;

namespace
{
	float ds = 0.075f;
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
	Graphics::SimpleDraw::Initialize(1000000);
	Graphics::Gui::Initialize(mWindow.GetWindowHandle());

	mSampler.Initialize(S::Graphics::Sampler::Filter::Point, S::Graphics::Sampler::AddressMode::Clamp);

	mConstantBuffer.Initialize(static_cast<uint32_t>(sizeof(CData)));

	mModelLoader.LoadOBJ("../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(sizeof(S::Graphics::Vertex), mTankMesh.GetVertices(), mTankMesh.GetVertexCount(), mTankMesh.GetIndices(), mTankMesh.GetIndexCount());
	mTankTexture[0].Initialize("../Assets/Models/Tank/tank_ao.jpg");
	mTankTexture[1].Initialize("../Assets/Models/Tank/tank_diffuse.jpg");
	mTankTexture[2].Initialize("../Assets/Models/Tank/tank_specular.jpg");
	mTankVertexShader.Initialize(L"../Assets/Shaders/Tank.fx", Graphics::Vertex::Format);
	mTankPixelShader.Initialize(L"../Assets/Shaders/Tank.fx");

	mTerrain.Initialize(L"../Assets/Shaders/PS_Terrain.fx", "../Assets/Images/Sand.jpg", "../Assets/Images/BlueCircle.png", 100, 100,3,10.0f);
	mSkyBox.Initialize(L"../Assets/Shaders/SkyBox.fx", "../Assets/Images/miramar_large.jpg", 1000.0f);

	mTankBound = Graphics::MeshBuilder::ComputeBound(mTankMesh);

	mCamera.SetPosition({ 0.0f,20.0f,-4.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,30.0f });

	/*
	mWorld.AddService<S::NavigationService>()->Setup(100u, 100u, 2.0f);
	mWorld.AddService<S::NavigationService>();
	mWorld.AddService<S::NavigationService>();
	mWorld.AddService<S::NavigationService>();
	mWorld.AddService<S::NavigationService>();
	mWorld.AddService<S::NavigationService>();
	mWorld.AddService<S::NavigationService>();
	...
	mWorld.Initialize(1000);
	*/
}

void GameApp::OnTerminate()
{
	//mWorld.Terminate();

	mSkyBox.Terminate();
	mTerrain.Terminate();

	mConstantBuffer.Terminate();

	mTankPixelShader.Terminate();
	mTankVertexShader.Terminate();
	for (int i = 0; i < 3; ++i)
	{
		mTankTexture[i].Terminate();
	}
	mTankMeshBuffer.Terminate();
	mTankMesh.Destroy();

	mSampler.Terminate();

	Graphics::SimpleDraw::Terminate();
	Graphics::Gui::Terminate();
	Graphics::SimpleDraw::Terminate();
	Input::InputSystem::StaticTerminate();
	mWindow.Terminate();
}

void GameApp::OnUpdate()
{
	Input::InputSystem* input = Input::InputSystem::Get();
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

	/*if (!mIsDragging)
	{
		if (input->IsMouseDown(Input::MouseButton::LBUTTON))
		{
			MouseRay1 = { (float)input->GetMouseScreenX(),(float)input->GetMouseScreenY() };
			MouseRay2 = { (float)input->GetMouseScreenX(),(float)input->GetMouseScreenY() };
			mIsDragging = true;
		}
	}

	if (!MouseRay1.x || !MouseRay1.y)
	{
		MouseRay1 = { (float)input->GetMouseScreenX(),(float)input->GetMouseScreenY() };
		MouseRay2 = { (float)input->GetMouseScreenX(),(float)input->GetMouseScreenY() };
	}
	else
	{
		MouseRay2 = { (float)input->GetMouseScreenX(),(float)input->GetMouseScreenY() };
	}
	X::DrawScreenRect({ MouseRay1.x,MouseRay1.y,MouseRay2.x,MouseRay2.y }, Math::Vector4::Cyan());

	l = MouseRay1.x < MouseRay2.x ? MouseRay1.x : MouseRay2.x;
	r = MouseRay1.x > MouseRay2.x ? MouseRay1.x : MouseRay2.x;
	t = MouseRay1.y < MouseRay2.y ? MouseRay1.y : MouseRay2.y;
	b = MouseRay1.y > MouseRay2.y ? MouseRay1.y : MouseRay2.y;*/



	// deltaTime
	static auto prevTime = std::chrono::high_resolution_clock::now();
	auto currTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count() / 1000.0f;
	prevTime = currTime;

	//mCamera.SetPosition(mTankPosition - mTankDirection * 10.0f + Math::Vector3{ 0.0f, 10.0f, 0.0f });
	//mCamera.SetLookAt(mTankPosition + Math::Vector3{ 0.0f, 5.0f, 0.0f });

	auto tankMoveSpeed = 5.0f;
	auto tankTurnSpeed = 1.0f;
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::UP))
		mTankPosition += mTankDirection * tankMoveSpeed * deltaTime;
	else if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::DOWN))
		mTankPosition -= mTankDirection * tankMoveSpeed * deltaTime;
	if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::RIGHT))
		mTankDirection = Math::TransformNormal(mTankDirection, Math::Matrix4::RotationY(tankTurnSpeed * deltaTime));
	else if (Input::InputSystem::Get()->IsKeyDown(Input::KeyCode::LEFT))
		mTankDirection = Math::TransformNormal(mTankDirection, Math::Matrix4::RotationY(-tankTurnSpeed * deltaTime));

	Graphics::GraphicsSystem::Get()->BeginRender();

	auto ray = mCamera.ScreenPointToRay(
		Input::InputSystem::Get()->GetMouseScreenX(),
		Input::InputSystem::Get()->GetMouseScreenY());
	float distance = 0.0f;
	if (mTerrain.Intersect(ray, distance))
	{
		auto intersectionPoint = ray.org + ray.dir * distance;
		Graphics::SimpleDraw::AddSphere(intersectionPoint, 5.0f, Math::Vector4::Cyan());
	}

	{
		auto bl = Math::Vector3{ mTankBound.center.x - mTankBound.extend.x, 0.0f, mTankBound.center.z - mTankBound.extend.z };
		auto tl = Math::Vector3{ mTankBound.center.x - mTankBound.extend.x, 0.0f, mTankBound.center.z + mTankBound.extend.z };
		auto br = Math::Vector3{ mTankBound.center.x + mTankBound.extend.x, 0.0f, mTankBound.center.z - mTankBound.extend.z };
		auto tr = Math::Vector3{ mTankBound.center.x + mTankBound.extend.x, 0.0f, mTankBound.center.z + mTankBound.extend.z };

		auto tankRotMat = Math::Matrix4::RotationY(atan2(mTankDirection.x, mTankDirection.z));
		bl = Math::TransformNormal(bl, tankRotMat) + mTankPosition;
		tl = Math::TransformNormal(tl, tankRotMat) + mTankPosition;
		br = Math::TransformNormal(br, tankRotMat) + mTankPosition;
		tr = Math::TransformNormal(tr, tankRotMat) + mTankPosition;

		float distance = 0.0f;
		Geometry::Ray ray = { { bl.x, 100.0f, bl.z }, -Math::Vector3::YAxis() };
		Graphics::SimpleDraw::AddLine(ray.org, ray.org + ray.dir * 1000.0f, Math::Vector4::Cyan());
		if (mTerrain.Intersect(ray, distance))
			bl.y = (ray.org + Math::Vector3{ ray.dir * distance }).y;
		ray = { { tl.x, 100.0f, tl.z }, -Math::Vector3::YAxis() };
		Graphics::SimpleDraw::AddLine(ray.org, ray.org + ray.dir * 1000.0f, Math::Vector4::Cyan());
		if (mTerrain.Intersect(ray, distance))
			tl.y = (ray.org + Math::Vector3{ ray.dir * distance }).y;
		ray = { { br.x, 100.0f, br.z }, -Math::Vector3::YAxis() };
		Graphics::SimpleDraw::AddLine(ray.org, ray.org + ray.dir * 1000.0f, Math::Vector4::Cyan());
		if (mTerrain.Intersect(ray, distance))
			br.y = (ray.org + Math::Vector3{ ray.dir * distance }).y;
		ray = { { tr.x, 100.0f, tr.z }, -Math::Vector3::YAxis() };
		Graphics::SimpleDraw::AddLine(ray.org, ray.org + ray.dir * 1000.0f, Math::Vector4::Cyan());
		if (mTerrain.Intersect(ray, distance))
			tr.y = (ray.org + Math::Vector3{ ray.dir * distance }).y;

		mTankPosition.y = (br.y + bl.y + tr.y + tl.y) * 0.25f;
		mTerrain.AddProjection(mTankPosition);

		auto l = Math::Normalize(Math::Vector3{ tr - br } +Math::Vector3{ tl - bl });
		auto r = Math::Normalize(Math::Vector3{ br - bl } +Math::Vector3{ tr - tl });
		auto u = Math::Cross(l, r);
		auto p = mTankPosition + Math::Vector3{ 0.0f, 3.0f, 0.0f };

		auto world = Math::Matrix4{
			r.x, r.y, r.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			l.x, l.y, l.z, 0.0f,
			p.x, p.y, p.z, 1.0f
		};

		Math::Matrix4 view, projection;

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

		mSampler.BindPS();

		mTankVertexShader.Bind();
		mTankPixelShader.Bind();
		//world = Math::Translation(mTankPosition);
		cd.w = Math::Transpose(world);
		mConstantBuffer.Set(&cd);
		mConstantBuffer.BindVS();
		mConstantBuffer.BindPS();

		for (int i = 0; i < 3; ++i)
		{
			mTankTexture[i].BindPS(i);
		}
		mSampler.BindPS();
		mTankMeshBuffer.Render();
	}

	//mWorld.Render();

	mSkyBox.Render(mCamera);
	mTerrain.Render(mCamera);
	/*S::Math::Vector3 a{ 80.0f,30.0f,50.0f };
	S::Math::Vector3 b{ 100.0f,70.0f,50.0f };
	S::Math::Vector3 c{ 120.0f,30.0f,50.0f };
	S::Graphics::SimpleDraw::AddLine(a, b, Math::Vector4::Red());
	S::Graphics::SimpleDraw::AddLine(b, c, Math::Vector4::Green());
	S::Graphics::SimpleDraw::AddLine(c, a, Math::Vector4::Blue());

	auto ray = mCamera.ScreenPointRay(S::Input::InputSystem::Get()->GetMouseScreenX(), S::Input::InputSystem::Get()->GetMouseScreenY());

	float distance{ 0.0f };
	if (Geometry::Intersect(ray, a, b, c, distance))
	{
		auto intersectionPoint = ray.org + ray.dir*distance;
		S::Graphics::SimpleDraw::AddSphere(intersectionPoint, 5.0f, Math::Vector4::Yellow());
	}*/

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