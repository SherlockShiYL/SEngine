#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>
#include <chrono>

class GameApp :public S::Core::Application
{
public:
	GameApp();
	~GameApp() override;

	void DepthDraw();

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate(bool(*GameLoop)(float)) override;

	S::Core::Window mWindow;
	S::Graphics::Camera mCamera;
	S::Graphics::Mesh mMesh;
	S::Graphics::Texture mTexture;
	S::Graphics::Texture mSelectionTexture;
	S::Graphics::Sampler mSampler;
	S::Graphics::MeshBuffer mMeshBuffer;
	S::Graphics::VertexShader mShadowVertexShader;
	S::Graphics::PixelShader mShadowPixelShader;
	S::Graphics::ConstantBuffer mConstantBuffer;
	S::Graphics::ConstantBuffer mTankConstantBuffer;

	S::Graphics::ModelLoader mModelLoader;
	S::Graphics::Mesh mTankMesh;
	S::Graphics::MeshBuffer mTankMeshBuffer;
	S::Graphics::Texture mTankTexture[3];
	S::Graphics::VertexShader mTankVertexShader;
	S::Graphics::PixelShader mTankPixelShader;


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

	S::Graphics::Camera mLightCamera;
	S::Graphics::VertexShader mDepthVertexShader;
	S::Graphics::PixelShader mDepthPixelShader;
	S::Graphics::RenderTarget mDepthRenderTarget;
	S::Graphics::ConstantBuffer mDepthConstantBuffer;

	S::Math::Vector3 mTankPosition{ 100.0f,25.0f,100.0f };

};

#endif // !INCLUDED_GAMEAPP_H