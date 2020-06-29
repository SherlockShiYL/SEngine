#include "Precompiled.h"
#include "SkyBox.h"

using namespace S;

void SkyBox::Initialize(const wchar_t* shader, const char* texture, float size)
{
	mConstantBuffer.Initialize();
	mTerrainVertexShader.Initialize(shader, Graphics::VertexPT::Format);
	mTerrainPixelShader.Initialize(shader);

	mSampler.Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Wrap);
	mTexture.Initialize(texture);

	Graphics::MeshBuilder::GenerateSkyBox(mMesh, size);
	mMeshBuffer.Initialize(mMesh, true);
}

void SkyBox::Terminate()
{
	mMeshBuffer.Terminate();
	mMesh.Destroy();
	mTexture.Terminate();
	mSampler.Terminate();

	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mConstantBuffer.Terminate();
}

void SkyBox::Render(const Graphics::Camera & camera, float aspect)
{
	auto world = Math::Translation(camera.GetPosition());
	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix(aspect);

	ConstantData data;
	data.world = Math::Transpose(world);
	data.view = Math::Transpose(view);
	data.projection = Math::Transpose(projection);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSampler.BindPS();
	mTexture.BindPS();

	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();

	mMeshBuffer.Render();
}
