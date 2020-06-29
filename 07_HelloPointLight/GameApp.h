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
	S::Graphics::MeshBuffer mMeshBuffer;

	S::Graphics::ConstantBuffer mConstantBuffer;

	S::Graphics::Camera mCamera;
	S::Graphics::MeshPNC mMeshPNC;
	S::Graphics::Sampler mSampler;

	S::Graphics::PointLight mPointLight{
		{ 0.0f,0.0f,-3.0f },
		0.4f,0.07f,0.007f,
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f,0.0f } };
	S::Graphics::Material mMaterial{
		{ 0.3f,0.3f,0.3f,0.0f },
		{ 0.7f,0.7f,0.7f,0.0f },
		{ 0.7f,0.7f,0.7f,0.0f },
		7.0f };
};

#endif // !INCLUDED_GAMEAPP_H