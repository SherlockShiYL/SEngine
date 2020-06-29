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

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	void DrawScene();
	void ProcessScene1();
	void ProcessScene2(int num);

	S::Core::Window mWindow;

	S::Graphics::VertexShader mVertexShader;
	S::Graphics::PixelShader mPixelShader;
	S::Graphics::VertexShader mPostProcessVertexShader;
	S::Graphics::PixelShader mPostProcessPixelShader;
	S::Graphics::RenderTarget mRenderTarget[3];

	S::Graphics::MeshBuffer mMeshBuffer;
	S::Graphics::ConstantBuffer mConstantBuffer;
	S::Graphics::ConstantBuffer mConstantBufferDis;

	S::Graphics::Camera mCamera;
	S::Graphics::Mesh mMesh;
	S::Graphics::Texture mTexture[4];
	S::Graphics::Sampler mSampler[2];
	S::Graphics::MeshBuffer mScreenMeshBuffer;

	S::Graphics::DirectionalLight mDirectionalLight{
		{ 1.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f } };
	S::Graphics::Material mMaterial{
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f },
		7.0f };
};

#endif // !INCLUDED_GAMEAPP_H