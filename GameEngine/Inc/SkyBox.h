#ifndef INCLUDED_GAMEENGINE_SKYBOX_H
#define INCLUDED_GAMEENGINE_SKYBOX_H

namespace S {

class SkyBox
{
public:
	SkyBox() = default;

	void Initialize(const wchar_t* shader, const char* texture, float size = 1000.0f);
	void Terminate();

	void Render(const Graphics::Camera& camera, float aspect = 0.0f);

private:
	struct ConstantData
	{
		Math::Matrix4 world;
		Math::Matrix4 view;
		Math::Matrix4 projection;
	};

	Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
	Graphics::VertexShader mTerrainVertexShader;
	Graphics::PixelShader mTerrainPixelShader;

	Graphics::Texture mTexture;
	Graphics::Sampler mSampler;

	Graphics::MeshPT mMesh;
	Graphics::MeshBuffer mMeshBuffer;

};

} // namesapce S

#endif // !INCLUDED_GAMEENGINE_SKYBOX_H