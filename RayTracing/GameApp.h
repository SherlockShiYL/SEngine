#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <GameEngine/Inc/GameEngine.h>
#include <chrono>

class GameApp :public S::Core::Application
{
public:
	GameApp();
	~GameApp() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate(bool(*GameLoop)(float)) override;

	S::Core::Window mWindow;
	S::Graphics::Camera mCamera;

	S::SkyBox mSkyBox;
	S::Terrain mTerrain;

	S::Graphics::RasterizerState mState;

	// Duck
	S::Graphics::Mesh mSphereMesh;
	S::Graphics::Texture mNormalTexture;
	S::Graphics::Sampler mSampler;
	S::Graphics::MeshBuffer mSphereMeshBuffer;
	S::Graphics::VertexShader mSphereVertexShader;
	S::Graphics::PixelShader mSpherePixelShader;
	S::Graphics::ConstantBuffer mSphereConstantBuffer;
	S::Math::Vector3 mSpherePosition{ 25.0f,40.0f,65.0f };

	// Model
	/*S::Graphics::AnimatedModel mModel;
	S::Graphics::ConstantBuffer mBoneConstantBuffer;
	S::Graphics::ConstantBuffer mBoneCB;*/

	S::Graphics::DirectionalLight mDirectionalLight{
		{ 1.0f,0.0f,0.0f },
		{ 0.75f,0.75f,0.75f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f } };
	S::Graphics::Material mMaterial{
		{ 0.75f,0.75f,0.75f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		7.0f };

	// Cube
	S::Graphics::Mesh mCubeMesh;
	S::Graphics::MeshBuffer mCubeMeshBuffer;
	S::Graphics::VertexShader mCubeVertexShader;
	S::Graphics::PixelShader mCubePixelShader;
	S::Graphics::ConstantBuffer mCubeConstantBuffer;
	S::Math::Vector3 mCubePosition{ 0.0f,50.0f,20.0f };
};

#endif // !INCLUDED_GAMEAPP_H