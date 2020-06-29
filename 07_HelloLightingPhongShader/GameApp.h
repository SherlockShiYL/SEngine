#ifndef INCLUDED_GAMEAPP_H
#define INCLUDED_GAMEAPP_H

#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>

class GameApp :public S::Core::Application
{
public:
	GameApp();
	~GameApp() override;

private:
	void OnInitialize() override;
	void OnTerminate() override;
	void OnUpdate() override;

	S::Core::Window mWindow;

	S::Graphics::VertexShader mVertexShader[3];
	S::Graphics::PixelShader mPixelShader[3];
	S::Graphics::MeshBuffer mMeshBuffer[2];

	S::Graphics::ConstantBuffer mConstantBuffer;

	S::Graphics::Texture mTexture;
	S::Graphics::Camera mCamera;
	S::Graphics::MeshPNC mMeshPNC;
	S::Graphics::MeshPNT mMeshPNT;
	S::Graphics::Sampler mSampler;

	S::Graphics::DirectionalLight mLight{
		{ 0.0f,0.0f,1.0f },
		{ 0.3f,0.3f,0.3f,0.0f },
		{ 0.7f,0.7f,0.7f,0.0f },
		{ 0.7f,0.7f,0.7f,0.0f } };
	S::Graphics::Material mMaterial{
		{ 0.3f,0.3f,0.3f,0.0f },
		{ 0.7f,0.7f,0.7f,0.0f },
		{ 0.7f,0.7f,0.7f,0.0f },
		50.0f };
};

#endif // !INCLUDED_GAMEAPP_H