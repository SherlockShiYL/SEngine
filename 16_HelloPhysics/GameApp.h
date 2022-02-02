#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <GameEngine/Inc/GameEngine.h>
#include <Physics/Inc/Physics.h>

class GameApp :public S::Core::Application
{
public:
	GameApp();
	~GameApp() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate(bool(*GameLoop)(float)) override;

	void Input();

	void GenerateCloak(S::Math::Vector2 pos, uint32_t rows, uint32_t columns);

	S::Core::Window mWindow;
	S::Graphics::Camera mCamera;

	S::SkyBox mSkyBox;

	S::Graphics::RasterizerState mState;

	S::Graphics::Sampler mSampler;

	// Cloak
	S::Graphics::Mesh mMesh;
	S::Graphics::MeshBuffer mMeshBuffer;
	S::Graphics::Texture mTexture;
	S::Graphics::VertexShader mVertexShader;
	S::Graphics::PixelShader mPixelShader;
	S::Graphics::ConstantBuffer mConstantBuffer;

	S::Graphics::DirectionalLight mLight{
	{ -1.0f, -1.0f, -1.0f },
	{ 1.0f,1.0f,1.0f,1.0f },
	{ 1.0f,1.0f,1.0f,1.0f },
	{ 1.0f,1.0f,1.0f,1.0f } };
	S::Graphics::Material mMaterial{
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		50.0f };

	S::Physics::Settings mSettings;
	S::Physics::PhysicsWorld physicsWorld;

};

#endif // !INCLUDED_GAMEAPP_H