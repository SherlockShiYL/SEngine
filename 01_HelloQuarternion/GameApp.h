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

	S::Graphics::Sampler mSampler;

	// Tank
	S::Graphics::ModelLoader mModelLoader;
	S::Graphics::Mesh mTankMesh;
	S::Graphics::MeshBuffer mTankMeshBuffer;
	S::Graphics::Texture mTankTexture[3];
	S::Graphics::VertexShader mTankVertexShader;
	S::Graphics::PixelShader mTankPixelShader;
	S::Graphics::ConstantBuffer mTankConstantBuffer;
	S::Math::Vector3 mTankPosition{ 20.0f,0.0f,20.0f };

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

	S::Math::Quaternion mQuaternion;
	S::Math::Matrix4 mRotateMatrix;
};

#endif // !INCLUDED_GAMEAPP_H