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

	S::Graphics::VertexShader mVertexShader;
	S::Graphics::PixelShader mPixelShader;
	S::Graphics::MeshBuffer mMeshBuffer[2];
	S::Graphics::ConstantBuffer mConstantBuffer;

	S::Graphics::Camera mCamera;
	S::Graphics::MeshPT mMeshPT[4];
	S::Graphics::Texture mTexture[4];
	S::Graphics::Sampler mSampler;
};

#endif // !INCLUDED_GAMEAPP_H