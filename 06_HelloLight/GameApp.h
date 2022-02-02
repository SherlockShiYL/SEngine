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
	void OnUpdate(bool(*GameLoop)(float)) override;

	S::Core::Window mWindow;

	S::Graphics::VertexShader mVertexShader;
	S::Graphics::PixelShader mPixelShader;
	S::Graphics::MeshBuffer mMeshBuffer;
	S::Graphics::ConstantBuffer mConstantBuffer;

	S::Graphics::Camera mCamera;
	S::Graphics::MeshPNT mMeshPNT;
	S::Graphics::Texture mTexture;
	S::Graphics::Sampler mSampler;

	S::Graphics::DirectionalLight mLight{ 
		{ 1.0f,0.0f,0.0f },
		{ 0.3f,0.3f,0.3f,1.0f },
		{ 0.7f,0.7f,0.7f,1.0f },
		{ 0.7f,0.7f,0.7f,1.0f } };
	S::Graphics::Material mMaterial{
		{ 0.3f,0.3f,0.3f,1.0f },
		{ 0.7f,0.7f,0.7f,1.0f },
		{ 0.7f,0.7f,0.7f,1.0f },
		50.0f };
};

#endif // !INCLUDED_GAMEAPP_H