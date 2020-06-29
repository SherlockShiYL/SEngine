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
	void OnUpdate() override;

	void GetBoneMatrices(S::Graphics::Bone* bone, float deltaTime);
	void DrawBone(S::Graphics::Bone* bone);

	void Input();

	void GenerateCloak(S::Math::Vector3 pos[], uint32_t rows, uint32_t columns, float restLength);

	S::Core::Window mWindow;
	S::Graphics::Camera mCamera;

	S::SkyBox mSkyBox;

	S::Graphics::RasterizerState mState;

	S::Graphics::Sampler mSampler;

	// Model
	S::Graphics::AnimatedModel mModel;
	S::Graphics::ConstantBuffer mBoneConstantBuffer;
	S::Graphics::ConstantBuffer mBoneCB;
	S::Math::Vector3 mCubePosition{ 0.0f,0.0f,0.0f };

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

	// Cloak
	S::Graphics::Mesh mMesh;
	S::Graphics::MeshBuffer mMeshBuffer;
	S::Graphics::Texture mTexture;
	S::Graphics::VertexShader mVertexShader;
	S::Graphics::PixelShader mPixelShader;
	S::Graphics::ConstantBuffer mConstantBuffer;

	S::Physics::Settings mSettings;
	S::Physics::PhysicsWorld physicsWorld;

	S::Geometry::OBB mOBBs[10];
};

#endif // !INCLUDED_GAMEAPP_H