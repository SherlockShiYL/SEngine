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

	S::Graphics::RasterizerState mState;

	S::Graphics::Sampler mSampler;

	// Cube
	S::Graphics::Mesh mCubeMesh;
	S::Graphics::MeshBuffer mCubeMeshBuffer;
	S::Graphics::Texture mCubeTexture;
	S::Graphics::VertexShader mCubeVertexShader;
	S::Graphics::PixelShader mCubePixelShader;
	S::Graphics::ConstantBuffer mCubeConstantBuffer;
	S::Math::Vector3 mCubePosition{ 0.0f,0.0f,0.0f };

	// BoneMesh
	S::Graphics::BoneMesh mBoneMesh;
	S::Graphics::MeshBuffer mBoneMeshBuffer;
	S::Graphics::VertexShader mBoneVertexShader;
	S::Graphics::PixelShader mBonePixelShader;
	S::Graphics::ConstantBuffer mBoneConstantBuffer;
	S::Math::Vector3 mBonePosition{ 5.0f,0.0f,0.0f };

	S::Math::Quaternion mQuaternion[2];
	S::Math::Matrix4 mRotateMatrix;

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

	// Bone
	struct Bone
	{
		Bone* parent{ nullptr };
		S::Math::Matrix4 transform;
	};
	std::vector<Bone*> mBones;
	std::vector<float> mBoneOffsets;
	std::vector<float> mBoneAngles; // one Axis
	std::vector<S::Math::Matrix4> mBoneWorldTransform;
	S::Math::Matrix4 toParentTransform;
};

#endif // !INCLUDED_GAMEAPP_H