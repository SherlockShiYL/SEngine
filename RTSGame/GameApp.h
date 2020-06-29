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
	void OnUpdate() override;

	S::Terrain mTerrain;
	S::SkyBox mSkyBox;

	S::Core::Window mWindow;
	S::Graphics::Camera mCamera;
	S::Graphics::Sampler mSampler;
	S::Graphics::ConstantBuffer mConstantBuffer;

	S::Graphics::ModelLoader mModelLoader;
	S::Graphics::Mesh mTankMesh;
	S::Graphics::MeshBuffer mTankMeshBuffer;
	S::Graphics::Texture mTankTexture[3];
	S::Graphics::VertexShader mTankVertexShader;
	S::Graphics::PixelShader mTankPixelShader;
	S::Math::Vector3 mTankPosition;
	S::Math::Vector3 mTankDirection{ 0.0f, 0.0f, 1.0f };
	S::Geometry::AABB mTankBound;

	S::Graphics::DirectionalLight mDirectionalLight{
		{ 1.0f,0.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f } };
	S::Graphics::Material mMaterial{
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		{ 1.0f,1.0f,1.0f,0.0f },
		7.0f };

	bool mIsDragging{ false };
	bool mStopDragging{ false };

	S::Math::Vector2 MouseRay1;
	S::Math::Vector2 MouseRay2;

	//S::World mWorld;
};

#endif // !INCLUDED_GAMEAPP_H